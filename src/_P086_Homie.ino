#ifdef USES_P086
//#######################################################################################################
//################################## Plugin 086: Homie receiver##########################################
//#######################################################################################################

#define P068_NO_LOG

#define PLUGIN_086
#define PLUGIN_ID_086         86
#define PLUGIN_NAME_086       "Generic - Homie receiver [TESTING]"

// empty default names because settings will be ignored / not used if value name is empty
#define PLUGIN_VALUENAME1_086 ""
#define PLUGIN_VALUENAME2_086 ""
#define PLUGIN_VALUENAME3_086 ""
#define PLUGIN_VALUENAME4_086 ""

#define PLUGIN_086_VALUE_INTEGER    0
#define PLUGIN_086_VALUE_FLOAT      1
#define PLUGIN_086_VALUE_BOOLEAN    2
#define PLUGIN_086_VALUE_STRING     3
#define PLUGIN_086_VALUE_ENUM       4
#define PLUGIN_086_VALUE_RGB        5
#define PLUGIN_086_VALUE_HSV        6
#define PLUGIN_086_BROADCAST        7
#define PLUGIN_086_SUBSCRIBE        8

#define PLUGIN_086_VALUE_TYPES      9
#define PLUGIN_086_VALUE_MAX        4

#define PLUGIN_086_DEBUG            true
#define PLUGIN_086_BROADCAST_TOPIC  "homie/$broadcast/%level%"

boolean Plugin_086(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {

    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_086;
        Device[deviceCount].Type = DEVICE_TYPE_DUMMY;
        Device[deviceCount].VType = SENSOR_TYPE_NONE;
        Device[deviceCount].Ports = 0;
        Device[deviceCount].PullUpOption = false;
        Device[deviceCount].InverseLogicOption = false;
        Device[deviceCount].FormulaOption = false;
        Device[deviceCount].DecimalsOnly = true;
        Device[deviceCount].ValueCount = PLUGIN_086_VALUE_MAX;
        Device[deviceCount].SendDataOption = false;
        Device[deviceCount].TimerOption = false;
        Device[deviceCount].GlobalSyncOption = false;
        Device[deviceCount].Custom = true;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_086);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_086));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_086));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[2], PSTR(PLUGIN_VALUENAME3_086));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[3], PSTR(PLUGIN_VALUENAME4_086));

        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        addFormNote(F("Translation Plugin for controllers able to receive value updates according to the Homie convention."));

        byte choice = 0;
        String labelText = "";
        String keyName = "";
        String options[PLUGIN_086_VALUE_TYPES];
        options[0] = F("integer");
        options[1] = F("float");
        options[2] = F("boolean");
        options[3] = F("string");
        options[4] = F("enum");
        options[5] = F("rgb");
        options[6] = F("hsv");
        options[7] = F("broadcast");
        options[8] = F("subscribe");
        int optionValues[PLUGIN_086_VALUE_TYPES];
        optionValues[0] = PLUGIN_086_VALUE_INTEGER;
        optionValues[1] = PLUGIN_086_VALUE_FLOAT;
        optionValues[2] = PLUGIN_086_VALUE_BOOLEAN;
        optionValues[3] = PLUGIN_086_VALUE_STRING;
        optionValues[4] = PLUGIN_086_VALUE_ENUM;
        optionValues[5] = PLUGIN_086_VALUE_RGB;
        optionValues[6] = PLUGIN_086_VALUE_HSV;
        optionValues[7] = PLUGIN_086_BROADCAST;
        optionValues[8] = PLUGIN_086_SUBSCRIBE;
        for (int i=0;i<PLUGIN_086_VALUE_MAX;i++) {
          labelText = F("Function #");
          labelText += (i+1);
          addFormSubHeader(labelText);
          choice = PCONFIG(i);
          if (i==0) addFormNote(F("Triggers an event when a ../%event%/set topic arrives"));
          labelText = F("Event Name");
          keyName = F("p086_functionName");
          keyName += i;
          addFormTextBox(labelText, keyName, ExtraTaskSettings.TaskDeviceValueNames[i], NAME_FORMULA_LENGTH_MAX);
          labelText = F("Parameter Type");
          keyName = F("p086_valueType");
          keyName += i;
          addFormSelector(labelText, keyName, PLUGIN_086_VALUE_TYPES, options, optionValues, choice );
          keyName += F("_min");
          addFormNumericBox(F("Min"),keyName,ExtraTaskSettings.TaskDevicePluginConfig[i]);
          keyName = F("p086_valueType");
          keyName += i;
          keyName += F("_max");
          addFormNumericBox(F("Max"),keyName,ExtraTaskSettings.TaskDevicePluginConfig[i+PLUGIN_086_VALUE_MAX]);
          if (i==0) addFormNote(F("min max values only valid for numeric parameter"));
          keyName = F("p086_decimals");
          keyName += i;
          addFormNumericBox(F("Decimals"),keyName,ExtraTaskSettings.TaskDeviceValueDecimals[i],0,8);
          if (i==0) addFormNote(F("Decimal counts for float parameter"));
          keyName = F("p086_string");
          keyName += i;
          addFormTextBox(F("Enum or Topic prefix"), keyName, ExtraTaskSettings.TaskDeviceFormula[i], NAME_FORMULA_LENGTH_MAX);
          if (i==0) addFormNote(F("Enumumeration list (comma seperated) or subscription topic."));
        }
        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        String keyName = "";
        for (int i=0;i<PLUGIN_086_VALUE_MAX;i++) {
          keyName = F("p086_valueType");
          keyName += i;
          PCONFIG(i) = getFormItemInt(keyName);
          keyName = F("p086_functionName");
          keyName += i;
          strncpy_webserver_arg(ExtraTaskSettings.TaskDeviceValueNames[i], keyName);
          keyName = F("p086_valueType");
          keyName += i;
          keyName += F("_min");
          ExtraTaskSettings.TaskDevicePluginConfig[i]=getFormItemInt(keyName);
          keyName = F("p086_valueType");
          keyName += i;
          keyName += F("_max");
          ExtraTaskSettings.TaskDevicePluginConfig[i+PLUGIN_086_VALUE_MAX]=getFormItemInt(keyName);
          keyName = F("p086_decimals");
          keyName += i;
          ExtraTaskSettings.TaskDeviceValueDecimals[i]=getFormItemInt(keyName);
          keyName = F("p086_string");
          keyName += i;
          strncpy_webserver_arg(ExtraTaskSettings.TaskDeviceFormula[i], keyName);
        }
        success = true;
        break;
      }

    case PLUGIN_GOT_CONNECTED:
      {
        String log = "";
        String subscribeTopic = "";
        bool subscribeResult = false;

        for (byte x=0; x<PLUGIN_086_VALUE_MAX;x++) {
          switch (Settings.TaskDevicePluginConfig[event->TaskIndex][x]) {
            case PLUGIN_086_BROADCAST:
              {
                LoadTaskSettings(event->TaskIndex);
                subscribeTopic = PLUGIN_086_BROADCAST_TOPIC;
                subscribeTopic.replace(F("%level%"), ExtraTaskSettings.TaskDeviceValueNames[x]);
                subscribeResult = MQTTclient.subscribe(subscribeTopic.c_str());
                #ifndef P086_NO_LOG
                if (loglevelActiveFor(LOG_LEVEL_INFO)) {
                  log = F("P086 : broadcast topic: ");
                  log += subscribeTopic;
                  if (subscribeResult) {
                    log += F(" subscribed.");
                  } else {
                    log += F(" subscribtion FAILED!");
                  }
                  addLog(LOG_LEVEL_INFO,log);
                }
                #endif
                success = subscribeResult;
                break;
              }
            case PLUGIN_086_SUBSCRIBE:
              {
                LoadTaskSettings(event->TaskIndex);
                subscribeTopic = ExtraTaskSettings.TaskDeviceFormula[x];
                subscribeResult = MQTTclient.subscribe(subscribeTopic.c_str());
                #ifndef P086_NO_LOG
                if (loglevelActiveFor(LOG_LEVEL_INFO)) {
                  log = F("P086 : subscribe topic: ");
                  log += subscribeTopic;
                  if (subscribeResult) {
                    log += F(" subscribed.");
                  } else {
                    log += F(" subscribtion FAILED!");
                  }
                  addLog(LOG_LEVEL_INFO,log);
                }
                #endif
                success = subscribeResult;
                break;
              }
            }
        }
        break;
      }

    case PLUGIN_READ:
      {
        for (byte x=0; x<PLUGIN_086_VALUE_MAX;x++)
        {
          String log = F("P086 : Value ");
          log += x+1;
          log += F(": ");
          log += UserVar[event->BaseVarIndex+x];
          addLog(LOG_LEVEL_INFO,log);
        }
        success = true;
        break;
      }

    case PLUGIN_WRITE:
      {
        String command = parseString(string, 1);
        if (command == F("homievalueset"))
        {
          if (event->Par1 == event->TaskIndex+1) {// make sure that this instance is the target
            LoadTaskSettings(event->Par1 -1);
            String parameter = parseStringToEndKeepCase(string,4);
            String log = "";
            float floatValue = 0;
            String enumList = "";
            int i = 0;
            #ifndef P086_NO_LOG
            if (loglevelActiveFor(LOG_LEVEL_INFO)) {
              log = F("P086 : deviceNr:");
              log += event->Par1;
              log += F(" valueNr:");
              log += event->Par2;
              log += F(" valueType:");
              log += Settings.TaskDevicePluginConfig[event->Par1-1][event->Par2-1];
            }
            #endif
            switch (Settings.TaskDevicePluginConfig[event->Par1-1][event->Par2-1]) {
              case PLUGIN_086_VALUE_INTEGER:
              case PLUGIN_086_VALUE_FLOAT:
                if (parameter!="") {
                  if (string2float(parameter,floatValue)) {
                    #ifndef P086_NO_LOG
                    if (loglevelActiveFor(LOG_LEVEL_INFO)) {
                      log += F(" integer/float set to ");
                      log += floatValue;
                      addLog(LOG_LEVEL_INFO,log);
                    }
                    #endif
                    UserVar[event->BaseVarIndex+event->Par2-1]=floatValue;
                  } else { // float conversion failed!
                    #ifndef P086_NO_LOG
                    if (loglevelActiveFor(LOG_LEVEL_ERROR)) {
                      log += F(" parameter:");
                      log += parameter;
                      log += F(" not a float value!");
                      addLog(LOG_LEVEL_ERROR,log);
                    }
                    #endif
                  }
                } else {
                  #ifndef P086_NO_LOG
                  if (loglevelActiveFor(LOG_LEVEL_INFO)) {
                    log += F(" value:");
                    log += UserVar[event->BaseVarIndex+event->Par2-1];
                    addLog(LOG_LEVEL_INFO,log);
                  }
                  #endif
                }
                break;

              case PLUGIN_086_VALUE_BOOLEAN:
                if (!string2float(parameter,floatValue)) {
                  floatValue = (parameter=="false") ? 0 : 1;
                }
                UserVar[event->BaseVarIndex+event->Par2-1]=floatValue;
                #ifndef P086_NO_LOG
                if (loglevelActiveFor(LOG_LEVEL_INFO)) {
                  log += F(" boolean set to ");
                  log += floatValue;
                  log += F(" (");
                  log += parameter;
                  log += F(")");
                  addLog(LOG_LEVEL_INFO,log);
                }
                #endif
                break;

              case PLUGIN_086_VALUE_STRING:
                //String values not stored to conserve flash memory
                //safe_strncpy(ExtraTaskSettings.TaskDeviceFormula[event->Par2-1], parameter.c_str(), sizeof(ExtraTaskSettings.TaskDeviceFormula[event->Par2-1]));
                #ifndef P086_NO_LOG
                if (loglevelActiveFor(LOG_LEVEL_INFO)) {
                  log += F(" string set to ");
                  log += parameter;
                  addLog(LOG_LEVEL_INFO,log);
                }
                #endif
                break;

              case PLUGIN_086_VALUE_ENUM:
                enumList = ExtraTaskSettings.TaskDeviceFormula[event->Par2-1];
                i = 1;
                while (parseStringKeepCase(enumList,i)!="") { // lookup result in enum List
                  if (parseStringKeepCase(enumList,i)==parameter) {
                    floatValue = i;
                    break;
                  }
                  i++;
                }
                UserVar[event->BaseVarIndex+event->Par2-1]=floatValue;
                #ifndef P086_NO_LOG
                if (loglevelActiveFor(LOG_LEVEL_INFO)) {
                  log += F(" enum set to ");
                  log += floatValue;
                  log += F(" (");
                  log += parameter;
                  log += F(") from ");
                  log += enumList;
                  addLog(LOG_LEVEL_INFO,log);
                }
                #endif
                break;

              case PLUGIN_086_VALUE_RGB:
                //String values not stored to conserve flash memory
                //safe_strncpy(ExtraTaskSettings.TaskDeviceFormula[event->Par2-1], parameter.c_str(), sizeof(ExtraTaskSettings.TaskDeviceFormula[event->Par2-1]));
                #ifndef P086_NO_LOG
                if (loglevelActiveFor(LOG_LEVEL_INFO)) {
                  log += F(" RGB received ");
                  log += parameter;
                  addLog(LOG_LEVEL_INFO,log);
                }
                #endif
                break;

              case PLUGIN_086_VALUE_HSV:
                //String values not stored to conserve flash memory
                //safe_strncpy(ExtraTaskSettings.TaskDeviceFormula[event->Par2-1], parameter.c_str(), sizeof(ExtraTaskSettings.TaskDeviceFormula[event->Par2-1]));
                #ifndef P086_NO_LOG
                if (loglevelActiveFor(LOG_LEVEL_INFO)) {
                  log += F(" HSV received ");
                  log += parameter;
                  addLog(LOG_LEVEL_INFO,log);
                }
                #endif
                break;
            }
            success = true;
          }
        }
        break;
      }
  }
  return success;
}
#endif // USES_P086
