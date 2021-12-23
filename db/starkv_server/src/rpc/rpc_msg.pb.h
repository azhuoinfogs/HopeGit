// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: rpc_msg.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_rpc_5fmsg_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_rpc_5fmsg_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3019000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3019001 < PROTOBUF_MIN_PROTOC_VERSION
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
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_rpc_5fmsg_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_rpc_5fmsg_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_rpc_5fmsg_2eproto;
class RpcMessage;
struct RpcMessageDefaultTypeInternal;
extern RpcMessageDefaultTypeInternal _RpcMessage_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::RpcMessage* Arena::CreateMaybeMessage<::RpcMessage>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class RpcMessage final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:RpcMessage) */ {
 public:
  inline RpcMessage() : RpcMessage(nullptr) {}
  ~RpcMessage() override;
  explicit constexpr RpcMessage(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  RpcMessage(const RpcMessage& from);
  RpcMessage(RpcMessage&& from) noexcept
    : RpcMessage() {
    *this = ::std::move(from);
  }

  inline RpcMessage& operator=(const RpcMessage& from) {
    CopyFrom(from);
    return *this;
  }
  inline RpcMessage& operator=(RpcMessage&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const RpcMessage& default_instance() {
    return *internal_default_instance();
  }
  static inline const RpcMessage* internal_default_instance() {
    return reinterpret_cast<const RpcMessage*>(
               &_RpcMessage_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(RpcMessage& a, RpcMessage& b) {
    a.Swap(&b);
  }
  inline void Swap(RpcMessage* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(RpcMessage* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  RpcMessage* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<RpcMessage>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const RpcMessage& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const RpcMessage& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(RpcMessage* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "RpcMessage";
  }
  protected:
  explicit RpcMessage(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kBodyMsgFieldNumber = 2,
    kHeadCodeFieldNumber = 1,
  };
  // optional bytes body_msg = 2 [default = ""];
  bool has_body_msg() const;
  private:
  bool _internal_has_body_msg() const;
  public:
  void clear_body_msg();
  const std::string& body_msg() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_body_msg(ArgT0&& arg0, ArgT... args);
  std::string* mutable_body_msg();
  PROTOBUF_NODISCARD std::string* release_body_msg();
  void set_allocated_body_msg(std::string* body_msg);
  private:
  const std::string& _internal_body_msg() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_body_msg(const std::string& value);
  std::string* _internal_mutable_body_msg();
  public:

  // optional uint32 head_code = 1 [default = 0];
  bool has_head_code() const;
  private:
  bool _internal_has_head_code() const;
  public:
  void clear_head_code();
  uint32_t head_code() const;
  void set_head_code(uint32_t value);
  private:
  uint32_t _internal_head_code() const;
  void _internal_set_head_code(uint32_t value);
  public:

  // @@protoc_insertion_point(class_scope:RpcMessage)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr body_msg_;
  uint32_t head_code_;
  friend struct ::TableStruct_rpc_5fmsg_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// RpcMessage

// optional uint32 head_code = 1 [default = 0];
inline bool RpcMessage::_internal_has_head_code() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool RpcMessage::has_head_code() const {
  return _internal_has_head_code();
}
inline void RpcMessage::clear_head_code() {
  head_code_ = 0u;
  _has_bits_[0] &= ~0x00000002u;
}
inline uint32_t RpcMessage::_internal_head_code() const {
  return head_code_;
}
inline uint32_t RpcMessage::head_code() const {
  // @@protoc_insertion_point(field_get:RpcMessage.head_code)
  return _internal_head_code();
}
inline void RpcMessage::_internal_set_head_code(uint32_t value) {
  _has_bits_[0] |= 0x00000002u;
  head_code_ = value;
}
inline void RpcMessage::set_head_code(uint32_t value) {
  _internal_set_head_code(value);
  // @@protoc_insertion_point(field_set:RpcMessage.head_code)
}

// optional bytes body_msg = 2 [default = ""];
inline bool RpcMessage::_internal_has_body_msg() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool RpcMessage::has_body_msg() const {
  return _internal_has_body_msg();
}
inline void RpcMessage::clear_body_msg() {
  body_msg_.ClearToEmpty();
  _has_bits_[0] &= ~0x00000001u;
}
inline const std::string& RpcMessage::body_msg() const {
  // @@protoc_insertion_point(field_get:RpcMessage.body_msg)
  return _internal_body_msg();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void RpcMessage::set_body_msg(ArgT0&& arg0, ArgT... args) {
 _has_bits_[0] |= 0x00000001u;
 body_msg_.SetBytes(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:RpcMessage.body_msg)
}
inline std::string* RpcMessage::mutable_body_msg() {
  std::string* _s = _internal_mutable_body_msg();
  // @@protoc_insertion_point(field_mutable:RpcMessage.body_msg)
  return _s;
}
inline const std::string& RpcMessage::_internal_body_msg() const {
  return body_msg_.Get();
}
inline void RpcMessage::_internal_set_body_msg(const std::string& value) {
  _has_bits_[0] |= 0x00000001u;
  body_msg_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* RpcMessage::_internal_mutable_body_msg() {
  _has_bits_[0] |= 0x00000001u;
  return body_msg_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* RpcMessage::release_body_msg() {
  // @@protoc_insertion_point(field_release:RpcMessage.body_msg)
  if (!_internal_has_body_msg()) {
    return nullptr;
  }
  _has_bits_[0] &= ~0x00000001u;
  auto* p = body_msg_.ReleaseNonDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (body_msg_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    body_msg_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  return p;
}
inline void RpcMessage::set_allocated_body_msg(std::string* body_msg) {
  if (body_msg != nullptr) {
    _has_bits_[0] |= 0x00000001u;
  } else {
    _has_bits_[0] &= ~0x00000001u;
  }
  body_msg_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), body_msg,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (body_msg_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    body_msg_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:RpcMessage.body_msg)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_rpc_5fmsg_2eproto