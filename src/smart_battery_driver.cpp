// Copyright 2020 Emerson Knapp
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>
#include <string>
#include <algorithm>

#include "sbs.hpp"
#include "visibility_control.h"

class SmartBatteryDriver
{
  public:
    explicit SmartBatteryDriver()
    {
      setvbuf(stdout, NULL, _IONBF, BUFSIZ);

      // TODO
      const std::string device_path = "";
      const int battery_address = 666;

      battery_ = std::make_unique<SBS::SmartBattery>(device_path.c_str(), battery_address);
    }

  private:
    uint8_t interpret_status(int status, float current) const
    {
      using SBS::BatteryStatus;
      if (status & static_cast<int>(BatteryStatus::Discharging)) {
        //return BatteryState::POWER_SUPPLY_STATUS_DISCHARGING;
      }
      // TODO(ek) figure out what this condition means
      // return POWER_SUPPLY_STATUS_NOT_CHARGING;
      if (status & static_cast<int>(BatteryStatus::FullyCharged)) {
        //return BatteryState::POWER_SUPPLY_STATUS_FULL;
      }
      if (current > 0) {
        //return BatteryState::POWER_SUPPLY_STATUS_CHARGING;
      }

      //return BatteryState::POWER_SUPPLY_STATUS_UNKNOWN;
    }

    uint8_t interpret_health(int status) const
    {
      using SBS::BatteryStatusAlarm;
      using SBS::BatteryStatus;

      if (status & static_cast<int>(BatteryStatusAlarm::OverTemp)) {
        //return BatteryState::POWER_SUPPLY_HEALTH_OVERHEAT;
      }
      if (status & static_cast<int>(BatteryStatusAlarm::OverCharged)) {
        //return BatteryState::POWER_SUPPLY_HEALTH_OVERVOLTAGE;
      }
      if (status & static_cast<int>(BatteryStatus::FullyDischarged)) {
        //return BatteryState::POWER_SUPPLY_HEALTH_DEAD;
      }
      if (status & static_cast<int>(BatteryStatusAlarm::Mask)) {
        // Any alarm set and haven't returned yet
        //return BatteryState::POWER_SUPPLY_HEALTH_UNSPEC_FAILURE;
      }

      //return BatteryState::POWER_SUPPLY_HEALTH_GOOD;
    }

    uint8_t interpret_chemistry(const std::string & chemistry)
    {
      std::string lower(chemistry);
      std::transform(lower.begin(), lower.end(), lower.begin(),
          [] (unsigned char c)
          {
            return std::tolower(c);
          });

      if (lower == "nimh") {
        //return BatteryState::POWER_SUPPLY_TECHNOLOGY_NIMH;
      } else if (lower == "lion") {
        //return BatteryState::POWER_SUPPLY_TECHNOLOGY_LION;
      } else if (lower == "lip") {
        //return BatteryState::POWER_SUPPLY_TECHNOLOGY_LIPO;
      } else if (lower == "nicd") {
        //return BatteryState::POWER_SUPPLY_TECHNOLOGY_NICD;
      } else {
        //return BatteryState::POWER_SUPPLY_TECHNOLOGY_UNKNOWN;
      }
    }

    void publish_battery_state()
    {
      //msg_ = std::make_unique<sensor_msgs::msg::BatteryState>();
      //msg_->header.stamp = get_clock()->now();
      //msg_->voltage = battery_->voltage() / 1000.0;
      //msg_->temperature = battery_->temperature();
      //msg_->current = battery_->current() / 1000.0;
      //msg_->charge = -msg_->current;
      //msg_->capacity = battery_->fullChargeCapacity() / 1000.0;
      //msg_->design_capacity = battery_->designCapacity() / 1000.0;
      //msg_->percentage = battery_->relativeStateOfCharge() / 100.0;

      int status = battery_->batteryStatus();
      //msg_->power_supply_status = interpret_status(status, msg_->current);
      //msg_->power_supply_health = interpret_health(status);

      std::string chemistry;
      battery_->cellChemistry(chemistry);
      //msg_->power_supply_technology = interpret_chemistry(chemistry);
      // TODO(ek) detect case when it's not
      //msg_->present = true;
      // msg->cell_voltage =
      // msg->location =
      //msg_->serial_number = std::to_string(battery_->serialNumber());
    }

  private:
    std::unique_ptr<SBS::SmartBattery> battery_;
};

int main(int argc, char ** argv)
{
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);
  auto driver = std::make_shared<SmartBatteryDriver>();

  // TODO: get correct bus number then device address
  // use address to query for device (RRC battery) state

  return 0;
}
