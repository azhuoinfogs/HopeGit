#ifndef METADATA_UTILS_H_
#define METADATA_UTILS_H_

#include <string>
#include <map>
#include "metadata.pb.h" // 包含protobuf生成的头文件
#include "boss_meta.h" // 包含protobuf生成的头文件

// 将C++对象转换为protobuf消息
boss_meta::Metadata toProtobuf(const struct Metadata& metadata);

// 将protobuf消息转换为C++对象
struct Metadata fromProtobuf(const boss_meta::Metadata& protoMetadata);

// 序列化元数据为字符串
std::string serializeMetadata(const boss_meta::Metadata& protoMetadata);

// 反序列化字符串为元数据
bool deserializeMetadata(const std::string& data, struct Metadata* metadata);

#endif // METADATA_UTILS_H_