
#ifndef _WS10_h
#define _WS10_h

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#if !defined (CONFIG_IDF_TARGET_ESP32P4)
#error "CONFIG_IDF_TARGET_ESP32P4 should be set"
#endif

#include "Panel_JD9365.h"

class LGFX : public lgfx::LGFX_Device
{
protected:

public:

    lgfx::Bus_MIPI       _bus_instance;
    lgfx::Panel_jd9365   _panel_instance; // NOTE: this panel does not use IBus
    lgfx::Light_PWM      _light_instance;
    lgfx::Touch_GT911    _touch_instance;

    LGFX(void)
    {
        std::uint32_t pkg_ver = lgfx::get_pkg_ver();
        ESP_LOGD("LGFX", "pkg_ver : %02x", (int)pkg_ver);

        {
            auto cfg = _bus_instance.config();
            cfg.panel = &_panel_instance;
            cfg.dsi_num_data_lanes = 2;
            cfg.dsi_lane_bit_rate_mbps = 1500; // mpbs

            cfg.ldo_chan_id = 3;
            cfg.ldo_voltage_mv = 2500;

            cfg.dbi_virtual_channel = 0;
            cfg.dbi_lcd_cmd_bits = 8;
            cfg.dbi_lcd_param_bits = 8;

            cfg.dpi_clock_freq_mhz = 60;

            cfg.hsync_back_porch = 20;
            cfg.hsync_pulse_width = 20;
            cfg.hsync_front_porch = 40;
            cfg.vsync_back_porch = 10;
            cfg.vsync_pulse_width = 4;
            cfg.vsync_front_porch = 30;

            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }


        {
            auto cfg = _panel_instance.config();
            cfg.panel_width = 800;
            cfg.panel_height = 1280;
            cfg.memory_width = 800;
            cfg.memory_height = 1280;
            cfg.readable = true;
            _panel_instance.config(cfg);
        }


        {
            auto cfg = _light_instance.config();
            cfg.pin_bl = GPIO_NUM_22;  // Backlight pin
            cfg.pwm_channel = 7;
            cfg.invert = false;
            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
        }

        setPanel(&_panel_instance);
    }


    bool init()
    {

        static constexpr int_fast16_t in_i2c_port = 0;// I2C_NUM_0;

        lgfx::i2c::init(in_i2c_port, GPIO_NUM_7, GPIO_NUM_8);
        return lgfx::LGFX_Device::init();
    }

    void setBrightness(uint8_t brightness) {
    }
};

#endif
//extern LGFX	gfx;
