# MqttWithWioLte
WioLTEを使ってMQTTでデータを飛ばします。

WioLTEは、待機時にSTM32F405の省電力モード（スタンバイモード）に入るため、5V50mA前後の待機電流となります。
通信時には、あらかじめタイマーでセットした時刻にマイコンが復帰し、LTE網にアタッチします。
通信終了後、再び省電力モードへ戻ります。


![overview image](https://github.com/poponta/MqttWithWioLte/blob/master/class.png)

# 注意事項

(Windowsの方は）ビルドをする際に、

C:\Users\USER\AppData\Local\Arduino15\packages\Seeeduino\hardware\Seeed_STM32F4\1.1.2\cores\arduino\libmaple\pwr.h

に対して、インクルードガードを付けてください。
（MacやLinuxの方も同様です）
~~~
#ifndef _PWR_H_
#define _PWR_H_

#include "libmaple.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Power interface register map. */
typedef struct pwr_reg_map {
    __io uint32 CR;      /**< Control register */
    __io uint32 CSR;     /**< Control and status register */
} pwr_reg_map;

/** Power peripheral register map base pointer. */
#define PWR_BASE                        ((struct pwr_reg_map*)0x40007000)

/*
 * Register bit definitions
 */

/* Control register */

/** Disable backup domain write protection bit */
#define PWR_CR_DBP  8
/** Power voltage detector enable bit */
#define PWR_CR_PVDE 4
/** Clear standby flag bit */
#define PWR_CR_CSBF 3
/** Clear wakeup flag bit */
#define PWR_CR_CWUF 2
/** Power down deepsleep bit */
#define PWR_CR_PDDS 1
/** Low-power deepsleep bit */
#define PWR_CR_LPDS 0

/* Control and status register */

/** Enable wakeup pin bit */
#define PWR_CSR_EWUP 8
/** PVD output bit */
#define PWR_CSR_PVDO 2
/** Standby flag bit */
#define PWR_CSR_SBF  1
/** Wakeup flag bit */
#define PWR_CSR_WUF  0

/*
 * Convenience functions
 */

void pwr_init(void);

#ifdef __cplusplus
}
#endif

#endif  // _PWR_H_
~~~
