/* Copyright (c) 2015, The Linux Foundation. All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 and
* only version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#ifndef __EMAC_PHY_H__
#define __EMAC_PHY_H__

#include <linux/platform_device.h>

struct emac_adapter;

struct emac_phy_ops {
	int  (*config)(struct platform_device *pdev, struct emac_adapter *adpt);
	int  (*init)(struct emac_adapter *adpt);
	void (*reset)(struct emac_adapter *adpt);
	int  (*init_ephy)(struct emac_adapter *adpt);
	int  (*up)(struct emac_adapter *adpt);
	void (*down)(struct emac_adapter *adpt);
	int  (*link_setup_no_ephy)(struct emac_adapter *adpt, u32 speed,
				   bool autoneg);
	int  (*link_check_no_ephy)(struct emac_adapter *adpt, u32 *speed,
				   bool *link_up);
	void (*tx_clk_set_rate)(struct emac_adapter *adpt);
	void (*periodic_task)(struct emac_adapter *adpt);
};

enum emac_flow_ctrl {
	EMAC_FC_NONE,
	EMAC_FC_RX_PAUSE,
	EMAC_FC_TX_PAUSE,
	EMAC_FC_FULL,
	EMAC_FC_DEFAULT
};

/* emac_phy
 * @addr mii address
 * @id vendor id
 * @cur_fc_mode flow control mode in effect
 * @req_fc_mode flow control mode requested by caller
 * @disable_fc_autoneg Do not auto-negotiate flow control
 */
struct emac_phy {
	int				phy_mode;
	bool				external;
	bool				uses_gpios;
	u32				addr;
	u16				id[2];
	bool				autoneg;
	u32				autoneg_advertised;
	u32				link_speed;
	bool				link_up;
	/* lock - synchronize access to mdio bus */
	spinlock_t			lock;
	struct emac_phy_ops		ops;
	void				*private;

	/* flow control configuration */
	enum emac_flow_ctrl		cur_fc_mode;
	enum emac_flow_ctrl		req_fc_mode;
	bool				disable_fc_autoneg;
};

int emac_phy_config(struct platform_device *pdev, struct emac_adapter *adpt);
int emac_phy_init_external(struct emac_adapter *adpt);
int emac_phy_read(struct emac_adapter *adpt, u16 phy_addr, u16 reg_addr,
		  u16 *phy_data);
int emac_phy_write(struct emac_adapter *adpt, u16 phy_addr, u16 reg_addr,
		   u16 phy_data);
int emac_phy_setup_link(struct emac_adapter *adpt, u32 speed, bool autoneg,
			bool fc);
int emac_phy_setup_link_speed(struct emac_adapter *adpt, u32 speed,
			      bool autoneg, bool fc);
int emac_phy_check_link(struct emac_adapter *adpt, u32 *speed, bool *link_up);
int emac_phy_get_lpa_speed(struct emac_adapter *adpt, u32 *speed);
int emac_phy_config_fc(struct emac_adapter *adpt);

#endif /* __EMAC_PHY_H__ */
