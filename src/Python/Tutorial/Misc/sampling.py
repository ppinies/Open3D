# Open3D: www.open3d.org
# The MIT License (MIT)
# See license file or visit www.open3d.org for details

from open3d import *
import os, sys
sys.path.append("../Utility")
from common import *
sys.path.append("../Advanced")
from trajectory_io import *
from shutil import copyfile

if __name__ == "__main__":
    set_verbosity_level(VerbosityLevel.Debug)

    path = "[path_to_reconstruction_system_output]"
    out_path = "[path_to_sampled_frames_are_located]"
    make_folder(out_path)
    make_folder(out_path + "depth/")
    make_folder(out_path + "image/")
    make_folder(out_path + "scene/")
    sampling_rate = 30

    depth_image_path = get_file_list(
            os.path.join(path, "depth/"), extension = ".png")
    color_image_path = get_file_list(
            os.path.join(path, "image/"), extension = ".jpg")
    pose_graph_global = read_pose_graph(path +
            template_global_posegraph_optimized)
    n_fragments = len(depth_image_path) // n_frames_per_fragment + 1
    pose_graph_fragments = []
    for i in range(n_fragments):
        pose_graph_fragment = read_pose_graph(path +
                template_fragment_posegraph_optimized % i)
        pose_graph_fragments.append(pose_graph_fragment)

    depth_image_path_new = []
    color_image_path_new = []
    traj = []
    cnt = 0
    for i in range(len(depth_image_path)):
        if i % sampling_rate == 0:
            metadata = [cnt, cnt, len(depth_image_path) // sampling_rate + 1]
            print(metadata)
            fragment_id = i // n_frames_per_fragment
            local_frame_id = i - fragment_id * n_frames_per_fragment
            traj.append(CameraPose(metadata, np.dot(
                    pose_graph_global.nodes[fragment_id].pose,
                    pose_graph_fragments[fragment_id].nodes[local_frame_id].pose)))
            copyfile(depth_image_path[i], out_path + "depth/" + \
                    os.path.basename(depth_image_path[i]))
            copyfile(color_image_path[i], out_path + "image/" + \
                    os.path.basename(color_image_path[i]))
            cnt += 1
    copyfile(path + "/scene/cropped.ply",
            out_path + "/scene/integrated.ply")
    write_trajectory(traj, out_path + "scene/key.log")
