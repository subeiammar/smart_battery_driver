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
#include <iostream>
#include <algorithm>

#include "sbs.hpp"
#include "visibility_control.h"

class SmartBatteryDriver
{
  public:
    explicit SmartBatteryDriver()
    {
      setvbuf(stdout, NULL, _IONBF, BUFSIZ);

      // TODO:
      // get device path using `i2cdetect -l`
      // get device address using `i2cdetect -y -r <bus_num>`
      const std::string device_path = "";
      const int battery_address = 666;

      battery_ = std::make_unique<SBS::SmartBattery>(device_path.c_str(), battery_address);
    }

    void publish_battery_state()
    {
      //msg_ = std::make_unique<sensor_msgs::msg::BatteryState>();
      //msg_->header.stamp = get_clock()->now();
      //const int charge = -msg_->current;
      
      const int voltage = battery_->voltage() / 1000; 
      const int capacity = battery_->fullChargeCapacity() / 1000;
      const int design_capacity = battery_->designCapacity() / 1000;
      const int percentage = battery_->relativeStateOfCharge() / 100;

      const float current = battery_->current() / 1000.0;
      const float temperature = battery_->temperature();

      int status = battery_->batteryStatus();

      const std::string serial_number = std::to_string(battery_->serialNumber());

      std::string chemistry;
      battery_->cellChemistry(chemistry);
      const std::string power_supply_technology = interpret_chemistry(chemistry);
      const std::string power_supply_status = interpret_status(status, current);
      const std::string power_supply_health = interpret_health(status);

      // TODO(ek) detect case when it's not present
      //msg_->present = true;
      // msg->cell_voltage =
      // msg->location =
      
      std::cout << std::endl;
      std::cout << "Voltage: " << voltage << "V "
        << "| Amperage: " << current << "A "
        << "| Percentage: " << percentage << "%" << std::endl;
      std::cout << "PS Status: " << power_supply_status << std::endl;
      std::cout << "PS Health: " << power_supply_health << std::endl;
      std::cout << std::endl;
    }

  private:
    std::string interpret_status(int status, float current) const
    {
      using SBS::BatteryStatus;
      std::string output = "Unknown";

      if (status & static_cast<int>(BatteryStatus::Discharging)) {
        output = "Discharging";
      }
      if (status & static_cast<int>(BatteryStatus::FullyCharged)) {
        output = "Full";
      }
      if (current > 0) {
        output += "|Charging";
      }

      // TODO(ek) figure out what this condition means
      // return POWER_SUPPLY_STATUS_NOT_CHARGING;

      return output;
    }

    std::string interpret_health(int status) const
    {
      using SBS::BatteryStatusAlarm;
      using SBS::BatteryStatus;
      std::string output = "Good";

      if (status & static_cast<int>(BatteryStatusAlarm::OverTemp)) {
        output = "Overheat";
      }
      if (status & static_cast<int>(BatteryStatusAlarm::OverCharged)) {
        output = "Overcharged";
      }
      if (status & static_cast<int>(BatteryStatus::FullyDischarged)) {
        output = "Dead/FullyDischarged";
      }
      if (status & static_cast<int>(BatteryStatusAlarm::Mask)) {
        // Any alarm set and haven't returned yet
        output = "Unspec?";
      }

      return output;
    }

    std::string interpret_chemistry(const std::string & chemistry)
    {
      std::string output(chemistry);
      std::transform(output.begin(), output.end(), output.begin(),
          [] (unsigned char c)
          {
            return std::tolower(c);
          });

      return output;
    }

  private:
    std::unique_ptr<SBS::SmartBattery> battery_;
};

int main(int argc, char ** argv)
{
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);
  auto driver = std::make_shared<SmartBatteryDriver>();

  driver->publish_battery_state();

  return 0;
}
