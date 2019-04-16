/*
 * Virtio nic PCI Bindings
 *
 * Copyright 2012 Red Hat, Inc.
 * Copyright 2012 Amit Shah <amit.shah@redhat.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or
 * (at your option) any later version.  See the COPYING file in the
 * top-level directory.
 */

#include "qemu/osdep.h"
#include <linux/kernel.h>
#include "virtio-pci.h"
#include "hw/virtio/virtio-nic.h"
#include "qapi/error.h"


/*
 * virtio-nic-pci: This extends VirtioPCIProxy.
 */
#define TYPE_VIRTIO_NIC_PCI "virtio-nic-pci-base"
typedef struct VirtIONicPCI VirtIONicPCI;

#define VIRTIO_NIC_PCI(obj) \
        OBJECT_CHECK(VirtIONicPCI, (obj), TYPE_VIRTIO_NIC_PCI)

struct VirtIONicPCI {
    VirtIOPCIProxy parent_obj;
    VirtIONIC vdev;
};

static void virtio_nic_pci_realize(VirtIOPCIProxy *vpci_dev, Error **errp)
{
    VirtIONicPCI *vnic = VIRTIO_NIC_PCI(vpci_dev);
    DeviceState *vdev = DEVICE(&vnic->vdev);
    Error *err = NULL;

    qdev_set_parent_bus(vdev, BUS(&vpci_dev->bus));
    object_property_set_bool(OBJECT(vdev), true, "realized", &err);
    if (err) {
        error_propagate(errp, err);
        return;
    }

    // object_property_set_link(OBJECT(vnic),
    //                          OBJECT(vnic->vdev.conf.rng), "rng",
    //                          NULL);
    printf("realization done in nic pic \n");
}

static void virtio_nic_pci_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    VirtioPCIClass *k = VIRTIO_PCI_CLASS(klass);
    PCIDeviceClass *pcidev_k = PCI_DEVICE_CLASS(klass);

    k->realize = virtio_nic_pci_realize;
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);

    pcidev_k->vendor_id = PCI_VENDOR_ID_REDHAT_QUMRANET;
    pcidev_k->device_id = PCI_DEVICE_ID_VIRTIO_NIC;
    pcidev_k->revision = VIRTIO_PCI_ABI_VERSION;
    pcidev_k->class_id = PCI_CLASS_OTHERS;
    printf("nic-pci.c : class init done \n");
   
}

static void virtio_nic_initfn(Object *obj)
{
    VirtIONicPCI *dev = VIRTIO_NIC_PCI(obj);

    virtio_instance_init_common(obj, &dev->vdev, sizeof(dev->vdev),
                                TYPE_VIRTIO_NIC);
    printf("nic-pci.c : instance init start \n");
}

static const VirtioPCIDeviceTypeInfo virtio_nic_pci_info = {
    .base_name             = TYPE_VIRTIO_NIC_PCI,
    .generic_name          = "virtio-nic-pci",
    .transitional_name     = "virtio-nic-pci-transitional",
    .non_transitional_name = "virtio-nic-pci-non-transitional",
    .instance_size = sizeof(VirtIONicPCI),
    .instance_init = virtio_nic_initfn,
    .class_init    = virtio_nic_pci_class_init,
    
};

static void virtio_nic_pci_register(void)
{
    printf("nic-pci.c : register start \n");
    virtio_pci_types_register(&virtio_nic_pci_info);
    printf("nic-pci.c : register end \n");
}

type_init(virtio_nic_pci_register)
