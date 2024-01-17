import subprocess
import os

def run_cmake(build_dir):
    # 确保 build 文件夹存在，如果不存在则创建
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)
    
        # 进入目标目录
    os.chdir(build_dir)
    
    # 构建CMake命令
    cmake_command = f'cmake -G "Visual Studio 17 Win64" ..\CMakeLists.txt'
    print(f"Running command: {cmake_command}")
    subprocess.run(cmake_command, shell=True)
    
    cmake_command = f'cmake -G "Visual Studio 17" -DFBX_STATIC_RTL=1 ..\CMakeLists.txt'
    print(f"Running command: {cmake_command}")
    subprocess.run(cmake_command, shell=True)
    
    cmake_command = f'cmake -G "Visual Studio 17 Win64" -DFBX_SHARED=1 ..\CMakeLists.txt'
    print(f"Running command: {cmake_command}")
    subprocess.run(cmake_command, shell=True)
    
    cmake_command = f'cmake -G "Visual Studio 17 Win64" -DFBX_SHARED=1 -DFBX_STATIC_RTL=1 ..\CMakeLists.txt'
    print(f"Running command: {cmake_command}")
    subprocess.run(cmake_command, shell=True)
    


# 获取当前脚本的路径
script_path = os.path.abspath(__file__)
# 获取当前脚本所在的目录
script_dir = os.path.dirname(script_path)
buildProj = os.path.join(script_dir,"build")

run_cmake(buildProj)