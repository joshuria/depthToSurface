#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <memory>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>


/**Print program usage and exit.*/
inline void printUsage() noexcept {
    std::cout << R"(
This utility will convert given KITTI raw point clould data (*.bin) to ply point cloud file (*.ply).

Usage:
    kittiScanToPly <Input binary point cloud file> <Output ply file name>

The input binary point cloud file should be from KITTI raw data and with file name pattern:
xxxxxxxxxx.bin.

The output ply file name (may with path) specifies where the result ply file to save. Note that we
will not append ply file extension if given file name does not have suitable file extension.
)";
    std::cout << std::endl;
}


int main(int const argc, char const* argv[]) {
    if (argc < 2) {
        printUsage();
        std::exit(0);
    }

    //! Debug output
    std::cout << "Loading file... \n";
    std::ifstream fin(argv[1], std::ios_base::binary);
    if (!fin) {
        std::cerr << "Fail to open KITTI binary point cloud file " << argv[1] << std::endl;
        std::exit(1);
    }
    std::ostringstream fileBuffer(std::ios_base::binary);
    fileBuffer << fin.rdbuf();
    fin.close();
    std::string buffer(fileBuffer.str());
    std::istringstream binRawData(buffer, std::ios_base::binary);

    //! As documented, KITTI's bin point cloud file contains all 32-bit float and each point has
    //! 4 such floats, so # of points is size of file (byte) / 4 / 4
    binRawData.seekg(0, std::ios_base::end);
    auto nPoints = binRawData.tellg() >> 4;
    binRawData.seekg(0, std::ios_base::beg);
    std::cout << "# of pts: " << nPoints << "\n";

    std::cout << "Processing ... ";
    pcl::PointCloud<pcl::PointXYZ> cloud;
    cloud.width = nPoints;
    cloud.height = 1;
    cloud.is_dense = false;
    cloud.points.resize(cloud.width * cloud.height);
    for (auto i = 0; i < nPoints; ++i) {
        float data[4];
        binRawData.read(reinterpret_cast<char*>(data), sizeof(data));
        cloud.points[i].x = data[0];
        cloud.points[i].y = data[1];
        cloud.points[i].z = data[2];
    }
    std::cout << "Done" << std::endl;

    std::cout << "Save to " << argv[2] << "\n";
    pcl::io::savePLYFileBinary(argv[2], cloud);

    return 0;
}

