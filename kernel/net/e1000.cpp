
#include "e1000.hpp"


void nic_pci_test(){
   pci::Device* nic_dev = nullptr;
    for (int i = 0; i < pci::num_device; ++i) {
      // NIC のクラスコードは 0x020000
      if (pci::devices[i].class_code.Match(0x02u, 0x00u, 0x00u)) {
        nic_dev = &pci::devices[i];
        // Intel 製の NIC を優先
        if (0x8086 == pci::ReadVendorId(*nic_dev)) {
          break;
        }
      }
    }

    if (nic_dev) {
      Log(kWarn, "NIC has been found: %d.%d.%d\n",
          nic_dev->bus, nic_dev->device, nic_dev->function);
    } else {
      Log(kError, "NIC has not been found\n");
      exit(1);
    }
}