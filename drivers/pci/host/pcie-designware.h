/*
 * Synopsys Designware PCIe host controller driver
 *
 * Copyright (C) 2013 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Author: Jingoo Han <jg1.han@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _PCIE_DESIGNWARE_H
#define _PCIE_DESIGNWARE_H

/*
 * Maximum number of MSI IRQs can be 256 per controller. But keep
 * it 128 as of now. If needed, then increment it in multiple of 32.
 */
#define MAX_MSI_IRQS			128
#define MAX_MSI_CTRLS			(MAX_MSI_IRQS / 32)

struct pcie_port {
	struct device		*dev;
	u8			root_bus_nr;
	void __iomem		*dbi_base;
	u64			cfg0_base;
	void __iomem		*va_cfg0_base;
	u32			cfg0_size;
	u64			cfg1_base;
	void __iomem		*va_cfg1_base;
	u32			cfg1_size;
	resource_size_t		io_base;
	phys_addr_t		io_bus_addr;
	u32			io_size;
	u64			mem_base;
	phys_addr_t		mem_bus_addr;
	u32			mem_size;
	int			cpu_addr_offset;
	struct resource		*cfg;
	struct resource		*io;
	struct resource		*mem;
	struct resource		*busn;
	int			irq;
	u32			lanes;
	u32			num_viewport;
	struct pcie_host_ops	*ops;
	int			msi_irq;
	struct irq_domain	*irq_domain;
	u64			msi_target;
	u8			iatu_unroll_enabled;
	unsigned int		msi_enable[MAX_MSI_CTRLS];
	DECLARE_BITMAP(msi_irq_in_use, MAX_MSI_IRQS);
};

struct pcie_host_ops {
	u32 (*readl_rc)(struct pcie_port *pp, u32 reg);
	void (*writel_rc)(struct pcie_port *pp, u32 reg, u32 val);
	int (*rd_own_conf)(struct pcie_port *pp, int where, int size, u32 *val);
	int (*wr_own_conf)(struct pcie_port *pp, int where, int size, u32 val);
	int (*rd_other_conf)(struct pcie_port *pp, struct pci_bus *bus,
			unsigned int devfn, int where, int size, u32 *val);
	int (*wr_other_conf)(struct pcie_port *pp, struct pci_bus *bus,
			unsigned int devfn, int where, int size, u32 val);
	int (*link_up)(struct pcie_port *pp);
	int (*host_init)(struct pcie_port *pp);
	void (*msi_set_irq)(struct pcie_port *pp, int irq);
	void (*msi_clear_irq)(struct pcie_port *pp, int irq);
	phys_addr_t (*get_msi_addr)(struct pcie_port *pp);
	u32 (*get_msi_data)(struct pcie_port *pp, int pos);
	void (*scan_bus)(struct pcie_port *pp);
	int (*msi_host_init)(struct pcie_port *pp, struct msi_controller *chip);
};

u32 dw_pcie_readl_rc(struct pcie_port *pp, u32 reg);
void dw_pcie_writel_rc(struct pcie_port *pp, u32 reg, u32 val);
int dw_pcie_cfg_read(void __iomem *addr, int size, u32 *val);
int dw_pcie_cfg_write(void __iomem *addr, int size, u32 val);
irqreturn_t dw_handle_msi_irq(struct pcie_port *pp);
void dw_pcie_msi_init(struct pcie_port *pp);
int dw_pcie_wait_for_link(struct pcie_port *pp);
void dw_pcie_msi_cfg_store(struct pcie_port *pp);
void dw_pcie_msi_cfg_restore(struct pcie_port *pp);
int dw_pcie_link_up(struct pcie_port *pp);
void dw_pcie_setup_rc(struct pcie_port *pp);
int dw_pcie_host_init(struct pcie_port *pp);
void dw_pcie_prog_outbound_atu(struct pcie_port *pp, int index,
		int type, u64 cpu_addr, u64 pci_addr, u32 size);

#endif /* _PCIE_DESIGNWARE_H */
