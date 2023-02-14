#include "esphome.h"
#include <EEPROMRollingCodeStorage.h>
#include <SomfyRemote.h>

#define COVER_UNKNOWN 0.5f

class SomfyArduinoCover : public Cover
{
private:
    SomfyRemote *remote;
    EEPROMRollingCodeStorage *storage;

public:
    uint32_t remoteCode;
    int eepromAddress;

    SomfyArduinoCover(byte emitterPin, uint32_t remoteCode, int eepromAddress)
        : Cover(), remoteCode(remoteCode), eepromAddress(eepromAddress)
    {
        set_device_class("shutter");
        storage = new EEPROMRollingCodeStorage(eepromAddress);
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
                remote->sendCommand(Command::Up);
            }
            else if (pos == COVER_CLOSED)
            {
                remote->sendCommand(Command::Down);
            }
        }
        if (call.get_stop())
        {
            remote->sendCommand(Command::My);
        }
        this->position = COVER_UNKNOWN;
        this->publish_state();
    }

    void program()
    {
        remote->sendCommand(Command::Prog);
    }

    int rollingCode()
    {
        uint16_t rollingCode;
        EEPROM.get(eepromAddress, rollingCode);
        return rollingCode;
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
            addCover(remoteCode + i);
        }
    }

    uint32_t remoteCodeFromMac()
    {
        uint8_t mac[6];
        get_mac_address_raw(mac);
        uint32_t remoteCode = (((uint32_t)mac[3]) << 16) | (((uint32_t)mac[4]) << 8) | ((uint32_t)mac[5]);
        return remoteCode;
    }

    void addCover(uint32_t remoteCode)
    {
        int eepromCodeSize = sizeof(uint16_t);
        auto cover = new SomfyArduinoCover(emitterPin, remoteCode, eepromCodeSize * covers.size());
        covers.push_back(cover);
    }
};
