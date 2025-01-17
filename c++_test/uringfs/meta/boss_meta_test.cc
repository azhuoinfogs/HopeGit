#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "boss_meta.h"
const char DBPath[] = "/opt/boss_meta";
int main(int argc, char* argv[]) {
    BossMeta boss_meta(DBPath);
    uint64_t id;
    boss_meta.getInodeIDByPath("/", &id);
    std::cout << id << std::endl;
    // 示例：创建目录和文件
    boss_meta.createEntry(id, "dir", true); // 创建目录
    uint64_t id1;
    boss_meta.getInodeIDByPath("/", &id1); // 创建目录
    std::cout << id1 << std::endl;
    assert(id1 == id);
    uint64_t id2;
    boss_meta.getInodeIDByPath("/dir", &id2); // 创建目录
    std::cout << id2 << std::endl;


    boss_meta.createEntry(id2, "example1.txt", false); // 创建文件
    boss_meta.createEntry(id2, "example2.txt", false, 255, 0644, 1000, 1000); // 设置文件大小为255字节，权限为0644，用户ID和组ID为1000
    
    uint64_t id3;
    std::string entryname3;
    boss_meta.getInodeIDByPath("/dir/example1.txt", &id3); // 创建目录
    boss_meta.getEntryNameByInode(id2, id3, &entryname3);
    std::cout << id3 << entryname3 <<std::endl;

    uint64_t id4;
    std::string entryname4;
    boss_meta.getInodeIDByPath("/dir/example2.txt", &id4); // 创建目录
    boss_meta.getEntryNameByInode(id2, id4, &entryname4);
    std::cout << id4 << entryname4 <<std::endl;

    // 初始化BossMeta实例，指定RocksDB数据库路径
    // // 示例：创建子目录和文件，并设置元数据
    boss_meta.createEntry(id, "documents", true);
    boss_meta.createEntry(id, "example.txt", false, 255, 0644, 1000, 1000); // 设置文件大小为255字节，权限为0644，用户ID和组ID为1000

    // // // 示例：更新文件内容并更新元数据
    // // bossMeta.updateFileContent("0", "example.txt", "Updated content!");

    // 示例：列出目录内容
    std::cout << "Listing root directory:\n";
    boss_meta.listEntry(id);
    std::cout << "=============================\n";
    // // 示例：删除文件或目录
    boss_meta.deleteEntry(id, "example.txt"); // 删除文件
    boss_meta.listEntry(id);
    std::cout << "=============================\n";
    boss_meta.deleteEntry(id, "dir"); // 删除子目录及其所有内容
    boss_meta.listEntry(id);
    std::cout << "=============================\n";
}