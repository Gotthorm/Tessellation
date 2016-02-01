# An import/export Blender plugin for OpenGL SuperBible SBM files

bl_info = {
    "name":         "OpenGL SuperBible SBM format",
    "author":       "Sean Butterworth",
    "blender":      (2,74,0),
    "version":      (6,0,0),
    "location":     "File > Import-Export",
    "description":  "Import-Export SBM",
    "support":		'TESTING',
    "category":     "Import-Export"
}

import os
import bpy
from bpy.props import ( BoolProperty, )
import struct
import mathutils
from bpy_extras.io_utils import ( 
    ExportHelper, 
    create_derived_objects, 
    free_derived_objects, 
    orientation_helper_factory,
    axis_conversion 
    )
from array import array

IO3DSOrientationHelper = orientation_helper_factory("IO3DSOrientationHelper", axis_forward='Y', axis_up='Z')

def WriteSBMHeader(file, objectCount):
    # Write the magic number 'SB6M'
    file.write(b"SB6M");

    # Write the size of this header.  
    # The loader uses this as an offset to the actual data
    file.write(struct.pack("<I", 16));

    # Write the number of objects we are exporting
    file.write(struct.pack("<I", objectCount));

    # Write the flags value.  Currently unused.
    file.write(struct.pack("<I", 0));

def WriteVertexAttributeChunk(file, objectCount):
    # Write the chnk name 'ATRB'
    file.write(b"ATRB");

    # Write the total size of this header and the attribute sub objects.  
    # The loader uses this as an offset to the next chunk
    file.write(struct.pack("<I", 84 * objectCount + 12));

    # Write the object count.  
    file.write(struct.pack("<I", objectCount));

def WriteVertexAttributeSubChunk(file, name, size, type, stride, flags, dataOffset):
    # The file space for the name is exactly 64 bytes
    maxSize = 64;

    # If the given name is greater than or equal to the maxSize, 
    # truncate it to the maxSize - 1
    if len(name) >= 64:
        name = name[:maxSize - 1];

    # Convert the string to bytes
    name = bytes(name, 'utf-8');

    # Write the name and pad it out to maxSize
    file.write(struct.pack("<%ds" % maxSize, name));

    # Write the size of the data type
    file.write(struct.pack("<I", size));

    # Write the data type
    file.write(struct.pack("<I", type));

    # Write the stride
    file.write(struct.pack("<I", stride));

    # Write the flags
    file.write(struct.pack("<I", flags));

    # Write the data offset
    file.write(struct.pack("<I", dataOffset));

def WriteVertexDataChunk(file, bytes, offset, vertCount):
    # Write the chnk name 'ATRB'
    file.write(b"VRTX");

    # Write the size of this block.  
    # The loader uses this as an offset to the actual data
    file.write(struct.pack("<I", 20));

    # Write the bytes
    file.write(struct.pack("<I", bytes));

    # Write the flags
    file.write(struct.pack("<I", offset));

    # Write the vert count
    file.write(struct.pack("<I", vertCount));

def WriteFloats(file, floatList):
    file.write(struct.pack('%sf' % len(floatList), *floatList));

def name_compat(name):
    if name is None:
        return 'None';
    else:
        return name.replace(' ', '_');

def mesh_triangulate(me):
    import bmesh
    bm = bmesh.new()
    bm.from_mesh(me)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(me)
    bm.free()

def veckey3d(v):
    return round(v.x, 4), round(v.y, 4), round(v.z, 4)

class ExportSBM(bpy.types.Operator, ExportHelper, IO3DSOrientationHelper):
    """Save a OpenGL SuperBible SBM File"""
    bl_idname       = "export_scene.sbm"
    bl_label        = "Export SBM"
    bl_options      = {'PRESET'}
    
    filename_ext    = ".sbm"
    
    use_selection = BoolProperty(name="Selection Only", description="Export selected objects only", default=False);

    def execute(self, context):

        # Exit edit mode before exporting, so current object states are exported properly.
        if bpy.ops.object.mode_set.poll():
            bpy.ops.object.mode_set(mode='OBJECT');

        global_matrix = axis_conversion(to_forward=self.axis_forward, to_up=self.axis_up).to_4x4();

        scene = context.scene;

        if use_selection:
            objects = (ob for ob in scene.objects if ob.is_visible(scene) and ob.select);
        else:
            objects = (ob for ob in scene.objects if ob.is_visible(scene));

        # Initialize totals, these are updated each object
        totverts = totuvco = totno = 1

        totalMeshList = [];
        totalNormalList = [];
        totalFaceVertexList = [];

        for i, ob_main in enumerate(objects):
            # ignore dupli children
            if ob_main.parent and ob_main.parent.dupli_type in {'VERTS', 'FACES'}:
                continue

            obs = []
            if ob_main.dupli_type != 'NONE':
                # XXX
                # print('creating dupli_list on', ob_main.name)
                ob_main.dupli_list_create(scene)

                obs = [(dob.object, dob.matrix) for dob in ob_main.dupli_list]

                # XXX debug print
                # print(ob_main.name, 'has', len(obs), 'dupli children')
            else:
                obs = [(ob_main, ob_main.matrix_world)]

            for ob, ob_mat in obs:
                uv_unique_count = no_unique_count = 0

                try:
                    me = ob.to_mesh(scene, True, 'PREVIEW', calc_tessface=False)
                except RuntimeError:
                    me = None

                if me is None:
                    continue

                me.transform(global_matrix * ob_mat)

                # _must_ do this first since it re-allocs arrays
                mesh_triangulate(me)

                me_verts = me.vertices[:]

                # Make our own list so it can be sorted to reduce context switching
                face_index_pairs = [(face, index) for index, face in enumerate(me.polygons)]
                # faces = [ f for f in me.tessfaces ]

                if not (len(face_index_pairs) + len(me.vertices)):  # Make sure there is something to write
                    # clean up
                    bpy.data.meshes.remove(me)
                    continue  # dont bother with this mesh.

                if face_index_pairs:
                    me.calc_normals_split()
                    # No need to call me.free_normals_split later, as this mesh is deleted anyway!
                    loops = me.loops
                else:
                    loops = []

                smooth_groups, smooth_groups_tot = (), 0

                materials = me.materials[:]
                material_names = [m.name if m else None for m in materials]

                # avoid bad index errors
                if not materials:
                    materials = [None]
                    material_names = [name_compat(None)]

                # Set the default mat to no material and no image.
                contextMat = 0, 0  # Can never be this, so we will label a new material the first chance we get.
                contextSmooth = None  # Will either be true or false,  set bad to force initialization switch.

                # Vert
                totalMeshList.extend(me_verts);

                # NORMAL, Smooth/Non smoothed.
                no_key = no_val = None
                normals_to_idx = {}
                no_get = normals_to_idx.get
                loops_to_normals = [0] * len(loops)
                for f, f_index in face_index_pairs:
                    for l_idx in f.loop_indices:
                        no_key = veckey3d(loops[l_idx].normal)
                        no_val = no_get(no_key)
                        if no_val is None:
                            no_val = normals_to_idx[no_key] = no_unique_count
                            no_unique_count += 1
                            totalNormalList.append(no_key);
                        loops_to_normals[l_idx] = no_val
                del normals_to_idx, no_get, no_key, no_val


                f_image = None

                for f, f_index in face_index_pairs:
                    f_smooth = f.use_smooth
                    if f_smooth and smooth_groups:
                        f_smooth = smooth_groups[f_index]
                    f_mat = min(f.material_index, len(materials) - 1)

                    key = material_names[f_mat], None  # No image, use None instead.

                    contextMat = key
                    if f_smooth != contextSmooth:
                        contextSmooth = f_smooth

                    f_v = [(vi, me_verts[v_idx], l_idx)
                            for vi, (v_idx, l_idx) in enumerate(zip(f.vertices, f.loop_indices))]

                    for vi, v, li in f_v:
                        totalFaceVertexList.append((totverts + v.index, totno + loops_to_normals[li]));

                # Make the indices global rather then per mesh
                totverts += len(me_verts)
                totuvco += uv_unique_count
                totno += no_unique_count

                # clean up
                bpy.data.meshes.remove(me)

        if ob_main.dupli_type != 'NONE':
            ob_main.dupli_list_clear()

        binaryFile = open(self.filepath, 'wb');

        WriteSBMHeader(binaryFile, 2);

        WriteVertexAttributeChunk(binaryFile, 2);

        WriteVertexAttributeSubChunk(binaryFile, 'position', 4, 0x1406, 0, 0, 0);

        totalVertices = len(totalFaceVertexList);
        totalVerticesBytes = totalVertices * 4 * 4;
        totalNormalBytes = totalVertices * 3 * 4;

        WriteVertexAttributeSubChunk(binaryFile, 'normal', 3, 0x1406, 0, 0, totalVerticesBytes);

        # (file, bytes, offset, vertCount)
        WriteVertexDataChunk(binaryFile, totalVerticesBytes + totalNormalBytes, 216, totalVertices);

        vertexList = [];
        normalList = [];

        for vertexIndex, normalIndex in totalFaceVertexList:
            vertexList.append(totalMeshList[vertexIndex - 1].co[:]);
            normalList.append(totalNormalList[normalIndex - 1]);

        for vertex in vertexList:
            WriteFloats(binaryFile, array('f',[vertex[0], vertex[1], vertex[2], 1.0]));

        for normal in normalList:
            WriteFloats(binaryFile, array('f',[normal[0], normal[1], normal[2]]));

        binaryFile.close();

        return {'FINISHED'};

def menu_func(self, context):
    self.layout.operator(ExportSBM.bl_idname, text="OpenGL SuperBible (.sbm)");

def register():
    bpy.utils.register_module(__name__);
    bpy.types.INFO_MT_file_export.append(menu_func);
    
def unregister():
    bpy.utils.unregister_module(__name__);
    bpy.types.INFO_MT_file_export.remove(menu_func);


if __name__ == "__main__":
    register()
