#include "esphome.h"
#include <RollingCodeStorage.h>
#include <SomfyRemote.h>

#define COVER_UNKNOWN 0.5f
#define TAG "somfy"

class SomfyArduinoCover : public Cover
{
private:
    SomfyRemote *remote;

public:
    uint32_t remoteCode;

    SomfyArduinoCover(byte emitterPin, uint32_t remoteCode, RollingCodeStorage *storage)
        : Cover(), remoteCode(remoteCode)
    {
        set_device_class("shutter");
        remote = new SomfyRemote(emitterPin, remoteCode, storage);
        remote->setup();
    }

    CoverTraits get_traits() override
    {
        auto traits = CoverTraits();
        traits.set_is_assumed_state(true);
        traits.set_supports_position(false);
        traits.set_supports_tilt(false);
        return traits;
    }

    void control(const CoverCall &call) override
    {
        if (call.get_position().has_value())
        {
            float pos = *call.get_position();
            if (pos == COVER_OPEN)
            {
                ESP_LOGD(TAG, "Remote code: %06X command: %02X", remoteCode, Command::Up);
                remote->sendCommand(Command::Up);
            }
            else if (pos == COVER_CLOSED)
            {
                ESP_LOGD(TAG, "Remote code: %06X command: %02X", remoteCode, Command::Down);
                remote->sendCommand(Command::Down);
            }
        }
        if (call.get_stop())
        {
            ESP_LOGD(TAG, "Remote code: %06X command: %02X", remoteCode, Command::My);
            remote->sendCommand(Command::My);
        }
        this->position = COVER_UNKNOWN;
        this->publish_state();
    }

    void program()
    {
        ESP_LOGD(TAG, "Remote code: %06X command: %02X", remoteCode, Command::Prog);
        remote->sendCommand(Command::Prog);
    }
};

struct RollingCode
{
    uint16_t nextCode = 0;
} PACKED; // NOLINT

class PreferencesRollingCodeStorage : public RollingCodeStorage
{
private:
    ESPPreferenceObject preferences;

public:
    PreferencesRollingCodeStorage(uint32_t remoteCode)
    {
        preferences = global_preferences->make_preference<RollingCode>(remoteCode, true);
    }
    uint16_t nextCode() override
    {
        RollingCode code;
        preferences.load(&code);
        ESP_LOGD(TAG, "Rolling code: %04X", code.nextCode);
        code.nextCode++;
        preferences.save(&code);
        return code.nextCode--;
    }
};

class SomfyArduinoRemote : public Component
{
private:
    byte emitterPin;

public:
    std::vector<esphome::cover::Cover *> covers;

    SomfyArduinoRemote(byte emitterPin, int coverCount = 0)
        : Component(), emitterPin(emitterPin)
    {
        uint32_t remoteCode = remoteCodeFromMac();
        for (int i = 0; i < coverCount; i++)
        {
            auto storage = new PreferencesRollingCodeStorage(remoteCode);
            auto cover = new SomfyArduinoCover(emitterPin, remoteCode, storage);
            covers.push_back(cover);
            remoteCode++;
        }
    }

    uint32_t remoteCodeFromMac()
    {
        uint8_t mac[6];
        get_mac_address_raw(mac);
        uint32_t remoteCode = (((uint32_t)mac[3]) << 16) | (((uint32_t)mac[4]) << 8) | ((uint32_t)mac[5]);
        return remoteCode;
    }
};
