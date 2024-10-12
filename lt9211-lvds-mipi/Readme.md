# LT9211C 转换配置

## 程序用法

执行`./xx.elf i2c-bus  lt9211-i2c-addr-7-Bit`

## 转换模式

修改`App/config.h`中的`LT9211C_MODE_SEL`

```c
#define         LT9211C_MODE_SEL             LVDS_IN_MIPI_OUT
/*
LVDS_IN_LVDS_OUT  LVDS_IN_MIPI_OUT  LVDS_IN_TTL_OUT
MIPI_IN_LVDS_OUT  MIPI_IN_MIPI_OUT  MIPI_IN_TTL_OUT
TTL_IN_LVDS_OUT   TTL_IN_MIPI_OUT   TTL_IN_TTL_OUT     //TTL_IN_TTL_OUT时需注意ttltx 8264寄存器配置
MIPI_REPEATER     MIPI_LEVEL_SHIFT  PATTERN_OUT
*/
```

此后，根据有关的配置，修改`Module`下对应的`.h`文件，一般`Mod..Rx.h``和`Mod..Tx.h`都需要修改时序（其中包括分辨率）：

```
ModLvdsRx.h  ModLvdsTx.h
ModMipiLs.h  ModMipiRpt.h
ModMipiRx.h  ModMipiTx.h
ModPattern.h  ModSystem.h
ModTtlRx.h  ModTtlTx.h
```

## 添加新的RX分辨率

> TX的分辨率是和RX统一的，修改了RX也就相当于修改了TX。

形如`VID_1920x1080_30Hz`分辨率一般已经有了。

如果想新增分辨率的话，可以先在`Ocm/type.h`里面添加对应的宏定义

```c
#define     VID_640x480_60Hz            0
#define     VID_720x480_60Hz            1
#define     VID_1280x720_60Hz           2
#define     VID_1366x768_60Hz           3
#define     VID_1280x720_30Hz           4
#define     VID_1920x720_60Hz           5
#define     VID_1920x1080_30Hz          6
#define     VID_1920x1080_60Hz          7
#define     VID_1920x1200_60Hz          8
#define     VID_3840x2160_30Hz          9
```

再搜索已有的代码，在`Driver/xx.c`里面依葫芦画瓢写结构体即可

```c
    #if LVDSRX_VID_SEL == VID_1920x1080_60Hz
    struct ChipRxVidTiming VidTiming = {88,    44,    148,    1920,    2200,    4,    5,   36,   1080,   1125,   148500};
    #endif
```

## 针对MIPI-TX点屏初始化序列


如果厂家给的代码是`SET_GENERIC(..)`，可以用`../tools/SET_GENERIC_to_lx.c`来生成龙讯所需的代码

> 这个工具修复了龙讯初始化序列某些命令不能直接使用的问题。
> ```c
>     SET_GENERIC(1);
>     W_D(0x29);
> ```
> 
> 需要转换为
> 
> ```c
>     SET_GENERIC(2);
>     W_D(0x29);
>     W_D(0x00);
> ```

将打印的结果（生成的代码） 依葫芦画瓢替换到`Driver/DrvDcsCmd.c`中即可。

## 调试流程

1. 一定先确保LT9211C的TX端的另外一头（例如RK，海思）已经开始接收例如MIPI、LVDS的数据

2. 确定龙讯供电、复位和I2C通信是否正常（能否读到版本号）

3. 检查配置的时序是否有问题

- 先将`LT9211C_MODE_SEL`改为`PATTERN_OUT`来验证TX端是否正常

- 再把`LT9211C_MODE_SEL`改为实际的转换场景，修改RX端的时序



