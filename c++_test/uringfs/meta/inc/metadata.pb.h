// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: metadata.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_metadata_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_metadata_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3014000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3014000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/map.h>  // IWYU pragma: export
#include <google/protobuf/map_entry.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_metadata_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_metadata_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_metadata_2eproto;
namespace boss_meta {
class Metadata;
class MetadataDefaultTypeInternal;
extern MetadataDefaultTypeInternal _Metadata_default_instance_;
class Metadata_XattrsEntry_DoNotUse;
class Metadata_XattrsEntry_DoNotUseDefaultTypeInternal;
extern Metadata_XattrsEntry_DoNotUseDefaultTypeInternal _Metadata_XattrsEntry_DoNotUse_default_instance_;
}  // namespace boss_meta
PROTOBUF_NAMESPACE_OPEN
template<> ::boss_meta::Metadata* Arena::CreateMaybeMessage<::boss_meta::Metadata>(Arena*);
template<> ::boss_meta::Metadata_XattrsEntry_DoNotUse* Arena::CreateMaybeMessage<::boss_meta::Metadata_XattrsEntry_DoNotUse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace boss_meta {

// ===================================================================

class Metadata_XattrsEntry_DoNotUse : public ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<Metadata_XattrsEntry_DoNotUse, 
    std::string, std::string,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING> {
public:
  typedef ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<Metadata_XattrsEntry_DoNotUse, 
    std::string, std::string,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING> SuperType;
  Metadata_XattrsEntry_DoNotUse();
  explicit Metadata_XattrsEntry_DoNotUse(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  void MergeFrom(const Metadata_XattrsEntry_DoNotUse& other);
  static const Metadata_XattrsEntry_DoNotUse* internal_default_instance() { return reinterpret_cast<const Metadata_XattrsEntry_DoNotUse*>(&_Metadata_XattrsEntry_DoNotUse_default_instance_); }
  static bool ValidateKey(std::string* s) {
    return ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(s->data(), static_cast<int>(s->size()), ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::PARSE, "boss_meta.Metadata.XattrsEntry.key");
 }
  static bool ValidateValue(std::string* s) {
    return ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(s->data(), static_cast<int>(s->size()), ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::PARSE, "boss_meta.Metadata.XattrsEntry.value");
 }
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& other) final;
  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_metadata_2eproto);
    return ::descriptor_table_metadata_2eproto.file_level_metadata[0];
  }

  public:
};

// -------------------------------------------------------------------

class Metadata PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:boss_meta.Metadata) */ {
 public:
  inline Metadata() : Metadata(nullptr) {}
  virtual ~Metadata();

  Metadata(const Metadata& from);
  Metadata(Metadata&& from) noexcept
    : Metadata() {
    *this = ::std::move(from);
  }

  inline Metadata& operator=(const Metadata& from) {
    CopyFrom(from);
    return *this;
  }
  inline Metadata& operator=(Metadata&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const Metadata& default_instance();

  static inline const Metadata* internal_default_instance() {
    return reinterpret_cast<const Metadata*>(
               &_Metadata_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(Metadata& a, Metadata& b) {
    a.Swap(&b);
  }
  inline void Swap(Metadata* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Metadata* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Metadata* New() const final {
    return CreateMaybeMessage<Metadata>(nullptr);
  }

  Metadata* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Metadata>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Metadata& from);
  void MergeFrom(const Metadata& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Metadata* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "boss_meta.Metadata";
  }
  protected:
  explicit Metadata(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_metadata_2eproto);
    return ::descriptor_table_metadata_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------


  // accessors -------------------------------------------------------

  enum : int {
    kXattrsFieldNumber = 14,
    kNameFieldNumber = 3,
    kParentInodeIdFieldNumber = 1,
    kInodeIdFieldNumber = 2,
    kFileSizeFieldNumber = 5,
    kAtimeFieldNumber = 6,
    kIsDirectoryFieldNumber = 4,
    kPermissionsFieldNumber = 8,
    kMtimeFieldNumber = 7,
    kOwnerIdFieldNumber = 9,
    kGroupIdFieldNumber = 10,
    kNlinkFieldNumber = 11,
    kBlockSizeFieldNumber = 12,
    kBlocksFieldNumber = 13,
  };
  // map<string, string> xattrs = 14;
  int xattrs_size() const;
  private:
  int _internal_xattrs_size() const;
  public:
  void clear_xattrs();
  private:
  const ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >&
      _internal_xattrs() const;
  ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >*
      _internal_mutable_xattrs();
  public:
  const ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >&
      xattrs() const;
  ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >*
      mutable_xattrs();

  // string name = 3;
  void clear_name();
  const std::string& name() const;
  void set_name(const std::string& value);
  void set_name(std::string&& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // uint64 parent_inode_id = 1;
  void clear_parent_inode_id();
  ::PROTOBUF_NAMESPACE_ID::uint64 parent_inode_id() const;
  void set_parent_inode_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_parent_inode_id() const;
  void _internal_set_parent_inode_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 inode_id = 2;
  void clear_inode_id();
  ::PROTOBUF_NAMESPACE_ID::uint64 inode_id() const;
  void set_inode_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_inode_id() const;
  void _internal_set_inode_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 file_size = 5;
  void clear_file_size();
  ::PROTOBUF_NAMESPACE_ID::uint64 file_size() const;
  void set_file_size(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_file_size() const;
  void _internal_set_file_size(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 atime = 6;
  void clear_atime();
  ::PROTOBUF_NAMESPACE_ID::uint64 atime() const;
  void set_atime(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_atime() const;
  void _internal_set_atime(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // bool is_directory = 4;
  void clear_is_directory();
  bool is_directory() const;
  void set_is_directory(bool value);
  private:
  bool _internal_is_directory() const;
  void _internal_set_is_directory(bool value);
  public:

  // uint32 permissions = 8;
  void clear_permissions();
  ::PROTOBUF_NAMESPACE_ID::uint32 permissions() const;
  void set_permissions(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_permissions() const;
  void _internal_set_permissions(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint64 mtime = 7;
  void clear_mtime();
  ::PROTOBUF_NAMESPACE_ID::uint64 mtime() const;
  void set_mtime(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_mtime() const;
  void _internal_set_mtime(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint32 owner_id = 9;
  void clear_owner_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 owner_id() const;
  void set_owner_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_owner_id() const;
  void _internal_set_owner_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 group_id = 10;
  void clear_group_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 group_id() const;
  void set_group_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_group_id() const;
  void _internal_set_group_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 nlink = 11;
  void clear_nlink();
  ::PROTOBUF_NAMESPACE_ID::uint32 nlink() const;
  void set_nlink(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_nlink() const;
  void _internal_set_nlink(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 block_size = 12;
  void clear_block_size();
  ::PROTOBUF_NAMESPACE_ID::uint32 block_size() const;
  void set_block_size(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_block_size() const;
  void _internal_set_block_size(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint64 blocks = 13;
  void clear_blocks();
  ::PROTOBUF_NAMESPACE_ID::uint64 blocks() const;
  void set_blocks(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_blocks() const;
  void _internal_set_blocks(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // @@protoc_insertion_point(class_scope:boss_meta.Metadata)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::MapField<
      Metadata_XattrsEntry_DoNotUse,
      std::string, std::string,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_STRING> xattrs_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::uint64 parent_inode_id_;
  ::PROTOBUF_NAMESPACE_ID::uint64 inode_id_;
  ::PROTOBUF_NAMESPACE_ID::uint64 file_size_;
  ::PROTOBUF_NAMESPACE_ID::uint64 atime_;
  bool is_directory_;
  ::PROTOBUF_NAMESPACE_ID::uint32 permissions_;
  ::PROTOBUF_NAMESPACE_ID::uint64 mtime_;
  ::PROTOBUF_NAMESPACE_ID::uint32 owner_id_;
  ::PROTOBUF_NAMESPACE_ID::uint32 group_id_;
  ::PROTOBUF_NAMESPACE_ID::uint32 nlink_;
  ::PROTOBUF_NAMESPACE_ID::uint32 block_size_;
  ::PROTOBUF_NAMESPACE_ID::uint64 blocks_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_metadata_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// Metadata

// uint64 parent_inode_id = 1;
inline void Metadata::clear_parent_inode_id() {
  parent_inode_id_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::_internal_parent_inode_id() const {
  return parent_inode_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::parent_inode_id() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.parent_inode_id)
  return _internal_parent_inode_id();
}
inline void Metadata::_internal_set_parent_inode_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  parent_inode_id_ = value;
}
inline void Metadata::set_parent_inode_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_parent_inode_id(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.parent_inode_id)
}

// uint64 inode_id = 2;
inline void Metadata::clear_inode_id() {
  inode_id_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::_internal_inode_id() const {
  return inode_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::inode_id() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.inode_id)
  return _internal_inode_id();
}
inline void Metadata::_internal_set_inode_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  inode_id_ = value;
}
inline void Metadata::set_inode_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_inode_id(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.inode_id)
}

// string name = 3;
inline void Metadata::clear_name() {
  name_.ClearToEmpty();
}
inline const std::string& Metadata::name() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.name)
  return _internal_name();
}
inline void Metadata::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.name)
}
inline std::string* Metadata::mutable_name() {
  // @@protoc_insertion_point(field_mutable:boss_meta.Metadata.name)
  return _internal_mutable_name();
}
inline const std::string& Metadata::_internal_name() const {
  return name_.Get();
}
inline void Metadata::_internal_set_name(const std::string& value) {
  
  name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline void Metadata::set_name(std::string&& value) {
  
  name_.Set(
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:boss_meta.Metadata.name)
}
inline void Metadata::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(value), GetArena());
  // @@protoc_insertion_point(field_set_char:boss_meta.Metadata.name)
}
inline void Metadata::set_name(const char* value,
    size_t size) {
  
  name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:boss_meta.Metadata.name)
}
inline std::string* Metadata::_internal_mutable_name() {
  
  return name_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* Metadata::release_name() {
  // @@protoc_insertion_point(field_release:boss_meta.Metadata.name)
  return name_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void Metadata::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:boss_meta.Metadata.name)
}

// bool is_directory = 4;
inline void Metadata::clear_is_directory() {
  is_directory_ = false;
}
inline bool Metadata::_internal_is_directory() const {
  return is_directory_;
}
inline bool Metadata::is_directory() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.is_directory)
  return _internal_is_directory();
}
inline void Metadata::_internal_set_is_directory(bool value) {
  
  is_directory_ = value;
}
inline void Metadata::set_is_directory(bool value) {
  _internal_set_is_directory(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.is_directory)
}

// uint64 file_size = 5;
inline void Metadata::clear_file_size() {
  file_size_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::_internal_file_size() const {
  return file_size_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::file_size() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.file_size)
  return _internal_file_size();
}
inline void Metadata::_internal_set_file_size(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  file_size_ = value;
}
inline void Metadata::set_file_size(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_file_size(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.file_size)
}

// uint64 atime = 6;
inline void Metadata::clear_atime() {
  atime_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::_internal_atime() const {
  return atime_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::atime() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.atime)
  return _internal_atime();
}
inline void Metadata::_internal_set_atime(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  atime_ = value;
}
inline void Metadata::set_atime(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_atime(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.atime)
}

// uint64 mtime = 7;
inline void Metadata::clear_mtime() {
  mtime_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::_internal_mtime() const {
  return mtime_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::mtime() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.mtime)
  return _internal_mtime();
}
inline void Metadata::_internal_set_mtime(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  mtime_ = value;
}
inline void Metadata::set_mtime(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_mtime(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.mtime)
}

// uint32 permissions = 8;
inline void Metadata::clear_permissions() {
  permissions_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Metadata::_internal_permissions() const {
  return permissions_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Metadata::permissions() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.permissions)
  return _internal_permissions();
}
inline void Metadata::_internal_set_permissions(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  permissions_ = value;
}
inline void Metadata::set_permissions(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_permissions(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.permissions)
}

// uint32 owner_id = 9;
inline void Metadata::clear_owner_id() {
  owner_id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Metadata::_internal_owner_id() const {
  return owner_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Metadata::owner_id() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.owner_id)
  return _internal_owner_id();
}
inline void Metadata::_internal_set_owner_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  owner_id_ = value;
}
inline void Metadata::set_owner_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_owner_id(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.owner_id)
}

// uint32 group_id = 10;
inline void Metadata::clear_group_id() {
  group_id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Metadata::_internal_group_id() const {
  return group_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Metadata::group_id() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.group_id)
  return _internal_group_id();
}
inline void Metadata::_internal_set_group_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  group_id_ = value;
}
inline void Metadata::set_group_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_group_id(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.group_id)
}

// uint32 nlink = 11;
inline void Metadata::clear_nlink() {
  nlink_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Metadata::_internal_nlink() const {
  return nlink_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Metadata::nlink() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.nlink)
  return _internal_nlink();
}
inline void Metadata::_internal_set_nlink(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  nlink_ = value;
}
inline void Metadata::set_nlink(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_nlink(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.nlink)
}

// uint32 block_size = 12;
inline void Metadata::clear_block_size() {
  block_size_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Metadata::_internal_block_size() const {
  return block_size_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 Metadata::block_size() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.block_size)
  return _internal_block_size();
}
inline void Metadata::_internal_set_block_size(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  block_size_ = value;
}
inline void Metadata::set_block_size(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_block_size(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.block_size)
}

// uint64 blocks = 13;
inline void Metadata::clear_blocks() {
  blocks_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::_internal_blocks() const {
  return blocks_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 Metadata::blocks() const {
  // @@protoc_insertion_point(field_get:boss_meta.Metadata.blocks)
  return _internal_blocks();
}
inline void Metadata::_internal_set_blocks(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  blocks_ = value;
}
inline void Metadata::set_blocks(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_blocks(value);
  // @@protoc_insertion_point(field_set:boss_meta.Metadata.blocks)
}

// map<string, string> xattrs = 14;
inline int Metadata::_internal_xattrs_size() const {
  return xattrs_.size();
}
inline int Metadata::xattrs_size() const {
  return _internal_xattrs_size();
}
inline void Metadata::clear_xattrs() {
  xattrs_.Clear();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >&
Metadata::_internal_xattrs() const {
  return xattrs_.GetMap();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >&
Metadata::xattrs() const {
  // @@protoc_insertion_point(field_map:boss_meta.Metadata.xattrs)
  return _internal_xattrs();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >*
Metadata::_internal_mutable_xattrs() {
  return xattrs_.MutableMap();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< std::string, std::string >*
Metadata::mutable_xattrs() {
  // @@protoc_insertion_point(field_mutable_map:boss_meta.Metadata.xattrs)
  return _internal_mutable_xattrs();
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace boss_meta

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_metadata_2eproto
