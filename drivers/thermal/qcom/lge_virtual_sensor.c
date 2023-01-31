// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
 */

#include <linux/thermal.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/err.h>
#include <linux/platform_device.h>

#include "lge_virtual_sensor.h"

#ifdef CONFIG_LGE_ONE_BINARY_SKU
#include <soc/qcom/lge/board_lge.h>
#endif

static const struct virtual_sensor_data sub6_lge_virtual_sensors[] = {
	/* 0.17*skin + 0.59*quiet + 6.76 */
	{
		.virt_zone_name = "vts-virt-therm",
		.num_sensors = 2,
		.sensor_names = {"skin-therm-usr", "quiet-therm-usr"},
		.coefficient_ct = 2,
		.coefficients = {17, 59},
		.avg_offset = 676000,
		.avg_denominator = 100,
		.logic = VIRT_WEIGHTED_AVG,
	},
};

static const struct virtual_sensor_data mmw_lge_virtual_sensors[] = {
	/* 0.17*skin + 0.59*quiet + 6.76 */
	{
		.virt_zone_name = "vts-virt-therm",
		.num_sensors = 2,
		.sensor_names = {"skin-therm-usr", "quiet-therm-usr"},
		.coefficient_ct = 2,
		.coefficients = {17, 59},
		.avg_offset = 676000,
		.avg_denominator = 100,
		.logic = VIRT_WEIGHTED_AVG,
	},

	/* qtm0(north) : 0.375*quiet-therm + 0.413*qtm-n-therm + 5.358 */
	{
		.virt_zone_name = "qtm-0-vts-therm",
		.num_sensors = 2,
		.sensor_names = {"quiet-therm-usr", "qtm-n-therm-usr"},
		.coefficient_ct = 2,
		.coefficients = {375, 413},
		.avg_offset = 5358000,
		.avg_denominator = 1000,
		.logic = VIRT_WEIGHTED_AVG,
	},
	/* qtm1(east) : -1.233*quiet-therm + 1.992*qtm-e-therm + 7.375 */
	{
		.virt_zone_name = "qtm-1-vts-therm",
		.num_sensors = 2,
		.sensor_names = {"quiet-therm-usr", "qtm-e-therm-usr"},
		.coefficient_ct = 2,
		.coefficients = {-1233, 1992},
		.avg_offset = 7375000,
		.avg_denominator = 1000,
		.logic = VIRT_WEIGHTED_AVG,
	},
	/* qtm2(west) : -0.332*quiet-therm + 1.050*qtm-w-therm + 8.913 */
	{
		.virt_zone_name = "qtm-2-vts-therm",
		.num_sensors = 2,
		.sensor_names = {"quiet-therm-usr", "qtm-w-therm-usr"},
		.coefficient_ct = 2,
		.coefficients = {-332, 1050},
		.avg_offset = 8913000,
		.avg_denominator = 1000,
		.logic = VIRT_WEIGHTED_AVG,
	},
	/* qtm-modem(smr) : -1.24*quiet-therm + 2.0*qtm-e-therm + 7.38 */
	{
		.virt_zone_name = "qtm-modem-vts-therm",
		.num_sensors = 2,
		.sensor_names = {"quiet-therm-usr", "qtm-e-therm-usr"},
		.coefficient_ct = 2,
		.coefficients = {-124, 200},
		.avg_offset = 738000,
		.avg_denominator = 100,
		.logic = VIRT_WEIGHTED_AVG,
	},
};


int lge_virtual_sensor_register(struct device *dev)
{
	int sens_ct = 0;
	static int idx;
	struct thermal_zone_device *tz;

#ifdef CONFIG_LGE_ONE_BINARY_SKU
	enum lge_sku_carrier_type sku_carrier = HW_SKU_MAX;
	sku_carrier = lge_get_sku_carrier();

	pr_info("operator is %s\n", sku_carrier == HW_SKU_NA_CDMA_VZW ? "VZW" : "Non-VZW");

	if (sku_carrier == HW_SKU_NA_CDMA_VZW){
		sens_ct = ARRAY_SIZE(mmw_lge_virtual_sensors);
		for (; idx < sens_ct; idx++) {
			tz = devm_thermal_of_virtual_sensor_register(dev,
					&mmw_lge_virtual_sensors[idx]);
			if (IS_ERR(tz))
				pr_err("%s: sensor:%s register error:%ld\n", __func__,
						mmw_lge_virtual_sensors[idx].virt_zone_name, PTR_ERR(tz));
			else
				pr_err("%s: sensor:%s register success\n", __func__,
					mmw_lge_virtual_sensors[idx].virt_zone_name);
		}
	} else {
		sens_ct = ARRAY_SIZE(sub6_lge_virtual_sensors);
		for (; idx < sens_ct; idx++) {
			tz = devm_thermal_of_virtual_sensor_register(dev,
					&sub6_lge_virtual_sensors[idx]);
			if (IS_ERR(tz))
				pr_err("%s: sensor:%s register error:%ld\n", __func__,
						sub6_lge_virtual_sensors[idx].virt_zone_name, PTR_ERR(tz));
			else
				pr_err("%s: sensor:%s register success\n", __func__,
						sub6_lge_virtual_sensors[idx].virt_zone_name);
		}
	}
#else
	sens_ct = ARRAY_SIZE(sub6_lge_virtual_sensors);
	for (; idx < sens_ct; idx++) {
		tz = devm_thermal_of_virtual_sensor_register(dev,
				&sub6_lge_virtual_sensors[idx]);
		if (IS_ERR(tz))
			pr_err("%s: sensor:%s register error:%ld\n", __func__,
					sub6_lge_virtual_sensors[idx].virt_zone_name, PTR_ERR(tz));
		else
			pr_err("%s: sensor:%s register success\n", __func__,
				sub6_lge_virtual_sensors[idx].virt_zone_name);
	}
#endif	//CONFIG_LGE_ONE_BINARY_SKU

	return 0;
}
