#include <NetlibApp/EndpointEnumerator/Linux/LinuxSerialPortScanner.hpp>

#include <libudev.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

namespace
{
   struct UdevDeleter
   {
      void operator()(udev *context) const noexcept
      {
         if (context != nullptr)
         {
            udev_unref(context);
         }
      }
   };

   struct UdevEnumerateDeleter
   {
      void operator()(udev_enumerate *enumerator) const noexcept
      {
         if (enumerator != nullptr)
         {
            udev_enumerate_unref(enumerator);
         }
      }
   };

   struct UdevDeviceDeleter
   {
      void operator()(udev_device *device) const noexcept
      {
         if (device != nullptr)
         {
            udev_device_unref(device);
         }
      }
   };

   [[nodiscard]] bool isUsbSerialDevice(udev_device *device) noexcept
   {
      return udev_device_get_parent_with_subsystem_devtype(device, "usb", "usb_device") != nullptr;
   }
}    // namespace

namespace netlib
{
   bool LinuxSerialPortScanner::isSerialDeviceNode(std::string_view deviceNode) noexcept
   {
      constexpr std::string_view prefix = "/dev/tty";
      if (!deviceNode.starts_with(prefix) || deviceNode == prefix ||
          deviceNode == "/dev/ttyprintk")
      {
         return false;
      }

      const auto suffix = deviceNode.substr(prefix.size());
      return !std::all_of(suffix.begin(), suffix.end(),
                          [](const char character)
                          {
                             return character >= '0' && character <= '9';
                          });
   }

   const std::vector<std::string> &LinuxSerialPortScanner::getAvailableSerialPorts()
   {
      m_ports.clear();

      const std::unique_ptr<udev, UdevDeleter> context(udev_new());
      if (context == nullptr)
      {
         std::cerr << "Unable to create libudev context for serial-port discovery." << std::endl;
         return m_ports;
      }

      const std::unique_ptr<udev_enumerate, UdevEnumerateDeleter> enumerator(
          udev_enumerate_new(context.get()));
      if (enumerator == nullptr ||
          udev_enumerate_add_match_subsystem(enumerator.get(), "tty") < 0 ||
          udev_enumerate_scan_devices(enumerator.get()) < 0)
      {
         std::cerr << "Unable to enumerate Linux serial ports through libudev." << std::endl;
         return m_ports;
      }

      udev_list_entry *entry = nullptr;
      udev_list_entry_foreach(entry, udev_enumerate_get_list_entry(enumerator.get()))
      {
         const char *const syspath = udev_list_entry_get_name(entry);
         const std::unique_ptr<udev_device, UdevDeviceDeleter> device(
             udev_device_new_from_syspath(context.get(), syspath));
         if (device == nullptr || !isUsbSerialDevice(device.get()))
         {
            continue;
         }

         const char *const deviceNode = udev_device_get_devnode(device.get());
         if (deviceNode != nullptr && isSerialDeviceNode(deviceNode))
         {
            m_ports.emplace_back(deviceNode);
         }
      }

      std::sort(m_ports.begin(), m_ports.end());
      m_ports.erase(std::unique(m_ports.begin(), m_ports.end()), m_ports.end());
      return m_ports;
   }
}    // namespace netlib
