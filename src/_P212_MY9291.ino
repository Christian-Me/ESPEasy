//#######################################################################################################
//#################################### Plugin 212: MY9291 HSV #########################################
//#######################################################################################################

// In addition to RGBW Values it can convert HSV to RGBW values.
// Update: ESPEasy can handle up to 5 Paramters insted of only 3.

// The MY9291 led driver is a device found in smart lightbulbs such as the AI Thinker AILight and many
// other bulbs that are either clones or licenced designs.  This plugin offers RGBW control of these
// bulbs set by HSV Values.

// The MY9291 library (from tinkerman) is required for this plugin to function correctly.
// https://github.com/xoseperez/my9291

// Although the MY9291 led driver can operate in 8bit, 16bit and 32bit modes, the default for the MY9291
// is used.  This offers 8bit control of each channel i.e. a 32bit command is sent on each change

// List of commands:
// (1) MY9291RGBW,<red 0-255>,<green 0-255>,<blue 0-255>,<white 0-255>
// (2) MY9291HSV,<Hue 0-360>,<Satuation 0-100>,<Value/Inensity 0-100>

// Usage:
// (1): Set RGBW Color to specified LED (eg. MY9291RGBW,255,255,255,20)
// (1): Set HSV Color to specified LED (eg. MY9291HSV,180,100,100)

#include <my9291.h>
my9291 *Plugin_212_rgbw;

#define PLUGIN_212
#define PLUGIN_ID_212         212
#define PLUGIN_NAME_212       "Output - MY9291 (RGBW)"
#define PLUGIN_VALUENAME1_212 "RGB"

boolean Plugin_212(byte function, struct EventStruct *event, String& string)
{
	boolean success = false;

	switch (function)
	{

	case PLUGIN_DEVICE_ADD:
	{
		Device[++deviceCount].Number = PLUGIN_ID_212;
		Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
		Device[deviceCount].Custom = true;
		Device[deviceCount].TimerOption = false;
		break;
	}

	case PLUGIN_GET_DEVICENAME:
	{
		string = F(PLUGIN_NAME_212);
		break;
	}

	case PLUGIN_GET_DEVICEVALUENAMES:
	{
		strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_212));
		break;
	}

	case PLUGIN_WEBFORM_LOAD:
	{

		addHtml(F("<TR><TD>DI Pin:<TD>"));
		addPinSelect(false, F("taskdevicepin1"), PCONFIG(1));
		addHtml(F("<TR><TD>DCKI Pin:<TD>"));
		addPinSelect(false, F("taskdevicepin2"), PCONFIG(2));

		success = true;
		break;
	}

	case PLUGIN_WEBFORM_SAVE:
	{
		PCONFIG(1) = getFormItemInt(F("taskdevicepin1"));
		PCONFIG(2) = getFormItemInt(F("taskdevicepin2"));
		success = true;
		break;

	}

	case PLUGIN_INIT:
	{
		if (!Plugin_212_rgbw)
		{
			Plugin_212_rgbw = new my9291(PCONFIG(1), PCONFIG(2), MY9291_COMMAND_DEFAULT);
			Plugin_212_rgbw->setState(true);
		}
		success = true;
		break;
	}

	case PLUGIN_WRITE:
	{
		if (Plugin_212_rgbw)
		{
			String log = "";
			if (loglevelActiveFor(LOG_LEVEL_INFO)) {
				log = F("P212 : ");
				log += string;
			}

			int rgbw[4];
			String tmpString  = string;
			int argIndex = tmpString.indexOf(',');
			if (argIndex)
				tmpString = tmpString.substring(0, argIndex);

			if (tmpString.equalsIgnoreCase(F("MY9291RGBW")))
			{
				Plugin_212_rgbw->setColor((my9291_color_t) { event->Par1, event->Par2, event->Par3, event->Par4 });
				Plugin_212_rgbw->setState(true);
				if (loglevelActiveFor(LOG_LEVEL_INFO)) {
					log += F(" RGBW:");
					log += event->Par1;
					log += ",";
					log += event->Par2;
					log += ",";
					log += event->Par3;
					log += ",";
					log += event->Par4;
					addLog(LOG_LEVEL_INFO,log);
				}
				success = true;
			}

			if (tmpString.equalsIgnoreCase(F("MY9291HSV")))
			{
				HSV2RGBW(event->Par1,event->Par2,event->Par3,rgbw);
				Plugin_212_rgbw->setColor((my9291_color_t) {rgbw[0],rgbw[1],rgbw[2],rgbw[3]});
				Plugin_212_rgbw->setState(true);
				if (loglevelActiveFor(LOG_LEVEL_INFO)) {
					log += F(" HSV ");
					log += event->Par1;
					log += ",";
					log += event->Par2;
					log += ",";
					log += event->Par3;
					log += F(" converted to RGB(W):");
					log += rgbw[0];
					log += ",";
					log += rgbw[1];
					log += ",";
					log += rgbw[2];
					log += ",";
					log += rgbw[3];
					addLog(LOG_LEVEL_INFO,log);
				}
				success = true;
			}

		}

		break;
	}

	}
	return success;
}
