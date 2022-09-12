// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_CONFIG_SENSACT_COMM_H_
#define FLATBUFFERS_GENERATED_CONFIG_SENSACT_COMM_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 2 &&
              FLATBUFFERS_VERSION_MINOR == 0 &&
              FLATBUFFERS_VERSION_REVISION == 8,
             "Non-compatible flatbuffers version included");

#include "_common_generated.h"

namespace sensact {
namespace comm {

struct tBlindConfig;
struct tBlindConfigBuilder;

struct tSinglePwmConfig;
struct tSinglePwmConfigBuilder;

struct tRgbwPwmConfig;
struct tRgbwPwmConfigBuilder;

struct tOnOffConfig;
struct tOnOffConfigBuilder;

struct tConfigWrapper;
struct tConfigWrapperBuilder;

struct tIoConfig;
struct tIoConfigBuilder;

enum uConfig : uint8_t {
  uConfig_NONE = 0,
  uConfig_tBlindConfig = 1,
  uConfig_tSinglePwmConfig = 2,
  uConfig_tOnOffConfig = 3,
  uConfig_tRgbwPwmConfig = 4,
  uConfig_MIN = uConfig_NONE,
  uConfig_MAX = uConfig_tRgbwPwmConfig
};

inline const uConfig (&EnumValuesuConfig())[5] {
  static const uConfig values[] = {
    uConfig_NONE,
    uConfig_tBlindConfig,
    uConfig_tSinglePwmConfig,
    uConfig_tOnOffConfig,
    uConfig_tRgbwPwmConfig
  };
  return values;
}

inline const char * const *EnumNamesuConfig() {
  static const char * const names[6] = {
    "NONE",
    "tBlindConfig",
    "tSinglePwmConfig",
    "tOnOffConfig",
    "tRgbwPwmConfig",
    nullptr
  };
  return names;
}

inline const char *EnumNameuConfig(uConfig e) {
  if (flatbuffers::IsOutRange(e, uConfig_NONE, uConfig_tRgbwPwmConfig)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesuConfig()[index];
}

template<typename T> struct uConfigTraits {
  static const uConfig enum_value = uConfig_NONE;
};

template<> struct uConfigTraits<sensact::comm::tBlindConfig> {
  static const uConfig enum_value = uConfig_tBlindConfig;
};

template<> struct uConfigTraits<sensact::comm::tSinglePwmConfig> {
  static const uConfig enum_value = uConfig_tSinglePwmConfig;
};

template<> struct uConfigTraits<sensact::comm::tOnOffConfig> {
  static const uConfig enum_value = uConfig_tOnOffConfig;
};

template<> struct uConfigTraits<sensact::comm::tRgbwPwmConfig> {
  static const uConfig enum_value = uConfig_tRgbwPwmConfig;
};

bool VerifyuConfig(flatbuffers::Verifier &verifier, const void *obj, uConfig type);
bool VerifyuConfigVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

struct tBlindConfig FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef tBlindConfigBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RELAY1 = 4,
    VT_RELAY2 = 6,
    VT_MODE = 8,
    VT_TIME_UP_MSECS = 10,
    VT_TIME_DOWN_MSECS = 12
  };
  uint16_t relay1() const {
    return GetField<uint16_t>(VT_RELAY1, 0);
  }
  uint16_t relay2() const {
    return GetField<uint16_t>(VT_RELAY2, 0);
  }
  sensact::comm::eRelayInterlockMode mode() const {
    return static_cast<sensact::comm::eRelayInterlockMode>(GetField<int8_t>(VT_MODE, 0));
  }
  uint32_t time_up_msecs() const {
    return GetField<uint32_t>(VT_TIME_UP_MSECS, 0);
  }
  uint32_t time_down_msecs() const {
    return GetField<uint32_t>(VT_TIME_DOWN_MSECS, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_RELAY1, 2) &&
           VerifyField<uint16_t>(verifier, VT_RELAY2, 2) &&
           VerifyField<int8_t>(verifier, VT_MODE, 1) &&
           VerifyField<uint32_t>(verifier, VT_TIME_UP_MSECS, 4) &&
           VerifyField<uint32_t>(verifier, VT_TIME_DOWN_MSECS, 4) &&
           verifier.EndTable();
  }
};

struct tBlindConfigBuilder {
  typedef tBlindConfig Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_relay1(uint16_t relay1) {
    fbb_.AddElement<uint16_t>(tBlindConfig::VT_RELAY1, relay1, 0);
  }
  void add_relay2(uint16_t relay2) {
    fbb_.AddElement<uint16_t>(tBlindConfig::VT_RELAY2, relay2, 0);
  }
  void add_mode(sensact::comm::eRelayInterlockMode mode) {
    fbb_.AddElement<int8_t>(tBlindConfig::VT_MODE, static_cast<int8_t>(mode), 0);
  }
  void add_time_up_msecs(uint32_t time_up_msecs) {
    fbb_.AddElement<uint32_t>(tBlindConfig::VT_TIME_UP_MSECS, time_up_msecs, 0);
  }
  void add_time_down_msecs(uint32_t time_down_msecs) {
    fbb_.AddElement<uint32_t>(tBlindConfig::VT_TIME_DOWN_MSECS, time_down_msecs, 0);
  }
  explicit tBlindConfigBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<tBlindConfig> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<tBlindConfig>(end);
    return o;
  }
};

inline flatbuffers::Offset<tBlindConfig> CreatetBlindConfig(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t relay1 = 0,
    uint16_t relay2 = 0,
    sensact::comm::eRelayInterlockMode mode = sensact::comm::eRelayInterlockMode_R1_UP__R2_DOWN,
    uint32_t time_up_msecs = 0,
    uint32_t time_down_msecs = 0) {
  tBlindConfigBuilder builder_(_fbb);
  builder_.add_time_down_msecs(time_down_msecs);
  builder_.add_time_up_msecs(time_up_msecs);
  builder_.add_relay2(relay2);
  builder_.add_relay1(relay1);
  builder_.add_mode(mode);
  return builder_.Finish();
}

struct tSinglePwmConfig FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef tSinglePwmConfigBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PWM_FIRST = 4,
    VT_PWM_LAST = 6,
    VT_ID_OF_STANDBY_CONTROLLER = 8,
    VT_AUTO_OFF_MSECS = 10
  };
  uint16_t pwm_first() const {
    return GetField<uint16_t>(VT_PWM_FIRST, 0);
  }
  uint16_t pwm_last() const {
    return GetField<uint16_t>(VT_PWM_LAST, 0);
  }
  uint16_t id_of_standby_controller() const {
    return GetField<uint16_t>(VT_ID_OF_STANDBY_CONTROLLER, 0);
  }
  uint32_t auto_off_msecs() const {
    return GetField<uint32_t>(VT_AUTO_OFF_MSECS, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_PWM_FIRST, 2) &&
           VerifyField<uint16_t>(verifier, VT_PWM_LAST, 2) &&
           VerifyField<uint16_t>(verifier, VT_ID_OF_STANDBY_CONTROLLER, 2) &&
           VerifyField<uint32_t>(verifier, VT_AUTO_OFF_MSECS, 4) &&
           verifier.EndTable();
  }
};

struct tSinglePwmConfigBuilder {
  typedef tSinglePwmConfig Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_pwm_first(uint16_t pwm_first) {
    fbb_.AddElement<uint16_t>(tSinglePwmConfig::VT_PWM_FIRST, pwm_first, 0);
  }
  void add_pwm_last(uint16_t pwm_last) {
    fbb_.AddElement<uint16_t>(tSinglePwmConfig::VT_PWM_LAST, pwm_last, 0);
  }
  void add_id_of_standby_controller(uint16_t id_of_standby_controller) {
    fbb_.AddElement<uint16_t>(tSinglePwmConfig::VT_ID_OF_STANDBY_CONTROLLER, id_of_standby_controller, 0);
  }
  void add_auto_off_msecs(uint32_t auto_off_msecs) {
    fbb_.AddElement<uint32_t>(tSinglePwmConfig::VT_AUTO_OFF_MSECS, auto_off_msecs, 0);
  }
  explicit tSinglePwmConfigBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<tSinglePwmConfig> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<tSinglePwmConfig>(end);
    return o;
  }
};

inline flatbuffers::Offset<tSinglePwmConfig> CreatetSinglePwmConfig(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t pwm_first = 0,
    uint16_t pwm_last = 0,
    uint16_t id_of_standby_controller = 0,
    uint32_t auto_off_msecs = 0) {
  tSinglePwmConfigBuilder builder_(_fbb);
  builder_.add_auto_off_msecs(auto_off_msecs);
  builder_.add_id_of_standby_controller(id_of_standby_controller);
  builder_.add_pwm_last(pwm_last);
  builder_.add_pwm_first(pwm_first);
  return builder_.Finish();
}

struct tRgbwPwmConfig FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef tRgbwPwmConfigBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PWM_R = 4,
    VT_PWM_G = 6,
    VT_PWM_B = 8,
    VT_PWM_WW = 10,
    VT_PWM_CW = 12,
    VT_ID_OF_STANDBY_CONTROLLER = 14,
    VT_AUTO_OFF_MSECS = 16
  };
  uint16_t pwm_r() const {
    return GetField<uint16_t>(VT_PWM_R, 0);
  }
  uint16_t pwm_g() const {
    return GetField<uint16_t>(VT_PWM_G, 0);
  }
  uint16_t pwm_b() const {
    return GetField<uint16_t>(VT_PWM_B, 0);
  }
  uint16_t pwm_ww() const {
    return GetField<uint16_t>(VT_PWM_WW, 0);
  }
  uint16_t pwm_cw() const {
    return GetField<uint16_t>(VT_PWM_CW, 0);
  }
  uint16_t id_of_standby_controller() const {
    return GetField<uint16_t>(VT_ID_OF_STANDBY_CONTROLLER, 0);
  }
  uint32_t auto_off_msecs() const {
    return GetField<uint32_t>(VT_AUTO_OFF_MSECS, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_PWM_R, 2) &&
           VerifyField<uint16_t>(verifier, VT_PWM_G, 2) &&
           VerifyField<uint16_t>(verifier, VT_PWM_B, 2) &&
           VerifyField<uint16_t>(verifier, VT_PWM_WW, 2) &&
           VerifyField<uint16_t>(verifier, VT_PWM_CW, 2) &&
           VerifyField<uint16_t>(verifier, VT_ID_OF_STANDBY_CONTROLLER, 2) &&
           VerifyField<uint32_t>(verifier, VT_AUTO_OFF_MSECS, 4) &&
           verifier.EndTable();
  }
};

struct tRgbwPwmConfigBuilder {
  typedef tRgbwPwmConfig Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_pwm_r(uint16_t pwm_r) {
    fbb_.AddElement<uint16_t>(tRgbwPwmConfig::VT_PWM_R, pwm_r, 0);
  }
  void add_pwm_g(uint16_t pwm_g) {
    fbb_.AddElement<uint16_t>(tRgbwPwmConfig::VT_PWM_G, pwm_g, 0);
  }
  void add_pwm_b(uint16_t pwm_b) {
    fbb_.AddElement<uint16_t>(tRgbwPwmConfig::VT_PWM_B, pwm_b, 0);
  }
  void add_pwm_ww(uint16_t pwm_ww) {
    fbb_.AddElement<uint16_t>(tRgbwPwmConfig::VT_PWM_WW, pwm_ww, 0);
  }
  void add_pwm_cw(uint16_t pwm_cw) {
    fbb_.AddElement<uint16_t>(tRgbwPwmConfig::VT_PWM_CW, pwm_cw, 0);
  }
  void add_id_of_standby_controller(uint16_t id_of_standby_controller) {
    fbb_.AddElement<uint16_t>(tRgbwPwmConfig::VT_ID_OF_STANDBY_CONTROLLER, id_of_standby_controller, 0);
  }
  void add_auto_off_msecs(uint32_t auto_off_msecs) {
    fbb_.AddElement<uint32_t>(tRgbwPwmConfig::VT_AUTO_OFF_MSECS, auto_off_msecs, 0);
  }
  explicit tRgbwPwmConfigBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<tRgbwPwmConfig> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<tRgbwPwmConfig>(end);
    return o;
  }
};

inline flatbuffers::Offset<tRgbwPwmConfig> CreatetRgbwPwmConfig(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t pwm_r = 0,
    uint16_t pwm_g = 0,
    uint16_t pwm_b = 0,
    uint16_t pwm_ww = 0,
    uint16_t pwm_cw = 0,
    uint16_t id_of_standby_controller = 0,
    uint32_t auto_off_msecs = 0) {
  tRgbwPwmConfigBuilder builder_(_fbb);
  builder_.add_auto_off_msecs(auto_off_msecs);
  builder_.add_id_of_standby_controller(id_of_standby_controller);
  builder_.add_pwm_cw(pwm_cw);
  builder_.add_pwm_ww(pwm_ww);
  builder_.add_pwm_b(pwm_b);
  builder_.add_pwm_g(pwm_g);
  builder_.add_pwm_r(pwm_r);
  return builder_.Finish();
}

struct tOnOffConfig FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef tOnOffConfigBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RELAY = 4,
    VT_AUTO_OFF_MSECS = 6,
    VT_INITIAL_STATE = 8
  };
  uint16_t relay() const {
    return GetField<uint16_t>(VT_RELAY, 0);
  }
  uint32_t auto_off_msecs() const {
    return GetField<uint32_t>(VT_AUTO_OFF_MSECS, 0);
  }
  sensact::comm::eOnOffState initial_state() const {
    return static_cast<sensact::comm::eOnOffState>(GetField<int8_t>(VT_INITIAL_STATE, 0));
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_RELAY, 2) &&
           VerifyField<uint32_t>(verifier, VT_AUTO_OFF_MSECS, 4) &&
           VerifyField<int8_t>(verifier, VT_INITIAL_STATE, 1) &&
           verifier.EndTable();
  }
};

struct tOnOffConfigBuilder {
  typedef tOnOffConfig Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_relay(uint16_t relay) {
    fbb_.AddElement<uint16_t>(tOnOffConfig::VT_RELAY, relay, 0);
  }
  void add_auto_off_msecs(uint32_t auto_off_msecs) {
    fbb_.AddElement<uint32_t>(tOnOffConfig::VT_AUTO_OFF_MSECS, auto_off_msecs, 0);
  }
  void add_initial_state(sensact::comm::eOnOffState initial_state) {
    fbb_.AddElement<int8_t>(tOnOffConfig::VT_INITIAL_STATE, static_cast<int8_t>(initial_state), 0);
  }
  explicit tOnOffConfigBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<tOnOffConfig> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<tOnOffConfig>(end);
    return o;
  }
};

inline flatbuffers::Offset<tOnOffConfig> CreatetOnOffConfig(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t relay = 0,
    uint32_t auto_off_msecs = 0,
    sensact::comm::eOnOffState initial_state = sensact::comm::eOnOffState_MANUAL_OFF) {
  tOnOffConfigBuilder builder_(_fbb);
  builder_.add_auto_off_msecs(auto_off_msecs);
  builder_.add_relay(relay);
  builder_.add_initial_state(initial_state);
  return builder_.Finish();
}

struct tConfigWrapper FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef tConfigWrapperBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CONFIG_TYPE = 4,
    VT_CONFIG = 6
  };
  sensact::comm::uConfig config_type() const {
    return static_cast<sensact::comm::uConfig>(GetField<uint8_t>(VT_CONFIG_TYPE, 0));
  }
  const void *config() const {
    return GetPointer<const void *>(VT_CONFIG);
  }
  template<typename T> const T *config_as() const;
  const sensact::comm::tBlindConfig *config_as_tBlindConfig() const {
    return config_type() == sensact::comm::uConfig_tBlindConfig ? static_cast<const sensact::comm::tBlindConfig *>(config()) : nullptr;
  }
  const sensact::comm::tSinglePwmConfig *config_as_tSinglePwmConfig() const {
    return config_type() == sensact::comm::uConfig_tSinglePwmConfig ? static_cast<const sensact::comm::tSinglePwmConfig *>(config()) : nullptr;
  }
  const sensact::comm::tOnOffConfig *config_as_tOnOffConfig() const {
    return config_type() == sensact::comm::uConfig_tOnOffConfig ? static_cast<const sensact::comm::tOnOffConfig *>(config()) : nullptr;
  }
  const sensact::comm::tRgbwPwmConfig *config_as_tRgbwPwmConfig() const {
    return config_type() == sensact::comm::uConfig_tRgbwPwmConfig ? static_cast<const sensact::comm::tRgbwPwmConfig *>(config()) : nullptr;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_CONFIG_TYPE, 1) &&
           VerifyOffset(verifier, VT_CONFIG) &&
           VerifyuConfig(verifier, config(), config_type()) &&
           verifier.EndTable();
  }
};

template<> inline const sensact::comm::tBlindConfig *tConfigWrapper::config_as<sensact::comm::tBlindConfig>() const {
  return config_as_tBlindConfig();
}

template<> inline const sensact::comm::tSinglePwmConfig *tConfigWrapper::config_as<sensact::comm::tSinglePwmConfig>() const {
  return config_as_tSinglePwmConfig();
}

template<> inline const sensact::comm::tOnOffConfig *tConfigWrapper::config_as<sensact::comm::tOnOffConfig>() const {
  return config_as_tOnOffConfig();
}

template<> inline const sensact::comm::tRgbwPwmConfig *tConfigWrapper::config_as<sensact::comm::tRgbwPwmConfig>() const {
  return config_as_tRgbwPwmConfig();
}

struct tConfigWrapperBuilder {
  typedef tConfigWrapper Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_config_type(sensact::comm::uConfig config_type) {
    fbb_.AddElement<uint8_t>(tConfigWrapper::VT_CONFIG_TYPE, static_cast<uint8_t>(config_type), 0);
  }
  void add_config(flatbuffers::Offset<void> config) {
    fbb_.AddOffset(tConfigWrapper::VT_CONFIG, config);
  }
  explicit tConfigWrapperBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<tConfigWrapper> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<tConfigWrapper>(end);
    return o;
  }
};

inline flatbuffers::Offset<tConfigWrapper> CreatetConfigWrapper(
    flatbuffers::FlatBufferBuilder &_fbb,
    sensact::comm::uConfig config_type = sensact::comm::uConfig_NONE,
    flatbuffers::Offset<void> config = 0) {
  tConfigWrapperBuilder builder_(_fbb);
  builder_.add_config(config);
  builder_.add_config_type(config_type);
  return builder_.Finish();
}

struct tIoConfig FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef tIoConfigBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TIMESTAMP = 4,
    VT_CONFIGS = 6
  };
  uint32_t timestamp() const {
    return GetField<uint32_t>(VT_TIMESTAMP, 0);
  }
  const flatbuffers::Vector<flatbuffers::Offset<sensact::comm::tConfigWrapper>> *configs() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<sensact::comm::tConfigWrapper>> *>(VT_CONFIGS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_TIMESTAMP, 4) &&
           VerifyOffset(verifier, VT_CONFIGS) &&
           verifier.VerifyVector(configs()) &&
           verifier.VerifyVectorOfTables(configs()) &&
           verifier.EndTable();
  }
};

struct tIoConfigBuilder {
  typedef tIoConfig Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_timestamp(uint32_t timestamp) {
    fbb_.AddElement<uint32_t>(tIoConfig::VT_TIMESTAMP, timestamp, 0);
  }
  void add_configs(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<sensact::comm::tConfigWrapper>>> configs) {
    fbb_.AddOffset(tIoConfig::VT_CONFIGS, configs);
  }
  explicit tIoConfigBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<tIoConfig> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<tIoConfig>(end);
    return o;
  }
};

inline flatbuffers::Offset<tIoConfig> CreatetIoConfig(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t timestamp = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<sensact::comm::tConfigWrapper>>> configs = 0) {
  tIoConfigBuilder builder_(_fbb);
  builder_.add_configs(configs);
  builder_.add_timestamp(timestamp);
  return builder_.Finish();
}

inline flatbuffers::Offset<tIoConfig> CreatetIoConfigDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t timestamp = 0,
    const std::vector<flatbuffers::Offset<sensact::comm::tConfigWrapper>> *configs = nullptr) {
  auto configs__ = configs ? _fbb.CreateVector<flatbuffers::Offset<sensact::comm::tConfigWrapper>>(*configs) : 0;
  return sensact::comm::CreatetIoConfig(
      _fbb,
      timestamp,
      configs__);
}

inline bool VerifyuConfig(flatbuffers::Verifier &verifier, const void *obj, uConfig type) {
  switch (type) {
    case uConfig_NONE: {
      return true;
    }
    case uConfig_tBlindConfig: {
      auto ptr = reinterpret_cast<const sensact::comm::tBlindConfig *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case uConfig_tSinglePwmConfig: {
      auto ptr = reinterpret_cast<const sensact::comm::tSinglePwmConfig *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case uConfig_tOnOffConfig: {
      auto ptr = reinterpret_cast<const sensact::comm::tOnOffConfig *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case uConfig_tRgbwPwmConfig: {
      auto ptr = reinterpret_cast<const sensact::comm::tRgbwPwmConfig *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyuConfigVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyuConfig(
        verifier,  values->Get(i), types->GetEnum<uConfig>(i))) {
      return false;
    }
  }
  return true;
}

inline const sensact::comm::tIoConfig *GettIoConfig(const void *buf) {
  return flatbuffers::GetRoot<sensact::comm::tIoConfig>(buf);
}

inline const sensact::comm::tIoConfig *GetSizePrefixedtIoConfig(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<sensact::comm::tIoConfig>(buf);
}

inline const char *tIoConfigIdentifier() {
  return "SCFG";
}

inline bool tIoConfigBufferHasIdentifier(const void *buf) {
  return flatbuffers::BufferHasIdentifier(
      buf, tIoConfigIdentifier());
}

inline bool SizePrefixedtIoConfigBufferHasIdentifier(const void *buf) {
  return flatbuffers::BufferHasIdentifier(
      buf, tIoConfigIdentifier(), true);
}

inline bool VerifytIoConfigBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<sensact::comm::tIoConfig>(tIoConfigIdentifier());
}

inline bool VerifySizePrefixedtIoConfigBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<sensact::comm::tIoConfig>(tIoConfigIdentifier());
}

inline void FinishtIoConfigBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<sensact::comm::tIoConfig> root) {
  fbb.Finish(root, tIoConfigIdentifier());
}

inline void FinishSizePrefixedtIoConfigBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<sensact::comm::tIoConfig> root) {
  fbb.FinishSizePrefixed(root, tIoConfigIdentifier());
}

}  // namespace comm
}  // namespace sensact

#endif  // FLATBUFFERS_GENERATED_CONFIG_SENSACT_COMM_H_
