/******************************************************************************************
 * Arduino-BG95-driver                                                                    *
 * ----------------------                                                                 *
 * Arduino driver for GSM/GPRS module BG95 to make HTTP/S connections                     *
 * with GET and POST methods                                                              *
 * Author: Cherlotte.hao                                                                  *
 * Last version available on https://github.com/CherLotte/Quectel_BG95-BG77-BG600L-driver *
 ******************************************************************************************
 * MIT License
 *
 * Copyright (c) 2024 Olivier Staquet
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************************/
#include "BG95.h"

/**
 * AT commands required (const char in PROGMEM to save memory usage)
 */
const char AT_CMD_BASE[] PROGMEM = "AT";                                      // Basic AT command to check the link
const char AT_CMD_ECHO[] PROGMEM = "ATE1&W";                                  // Set command echo mode

const char AT_CMD_CPIN_TEST[] PROGMEM = "AT+CPIN?";                           // Check SIM card status
const char AT_CMD_CPIN_PIN[] PROGMEM = "AT+CPIN=";                            // Configure PIN code

const char AT_CMD_CSQ[] PROGMEM = "AT+CSQ";                                   // Check the signal strengh
const char AT_CMD_ATI[] PROGMEM = "ATI";                                      // Output version of the module
const char AT_CMD_GMR[] PROGMEM = "AT+GMR";                                   // Output version of the firmware
const char AT_CMD_SIM_CARD[] PROGMEM = "AT+CCID";						                  // Get Sim Card version

const char AT_CMD_CFUN_TEST[] PROGMEM = "AT+CFUN?";                           // Check the current power mode
const char AT_CMD_CFUN0[] PROGMEM = "AT+CFUN=0";                              // Switch minimum power mode
const char AT_CMD_CFUN1[] PROGMEM = "AT+CFUN=1";                              // Switch normal power mode
const char AT_CMD_CFUN4[] PROGMEM = "AT+CFUN=4";                              // Switch sleep power mode

const char AT_CMD_CREG_TEST[] PROGMEM = "AT+CREG?";                           // Check the network registration status
const char AT_CMD_SAPBR_GPRS[] PROGMEM = "AT+SAPBR=3,1,\"Contype\",\"GPRS\""; // Configure the GPRS bearer
const char AT_CMD_SAPBR_APN[] PROGMEM = "AT+SAPBR=3,1,\"APN\",";              // Configure the APN for the GPRS
const char AT_CMD_SAPBR_USER[] PROGMEM = "AT+SAPBR=3,1,\"USER\",";            // Configure the USER for the GPRS (linked to APN)
const char AT_CMD_SAPBR_PWD[] PROGMEM = "AT+SAPBR=3,1,\"PWD\",";              // Configure the PWD for the GPRS (linked to APN)
const char AT_CMD_SAPBR1[] PROGMEM = "AT+SAPBR=1,1";                          // Connect GPRS
const char AT_CMD_SAPBR2[] PROGMEM = "AT+SAPBR=2,1";                          // Check GPRS connection status
const char AT_CMD_SAPBR0[] PROGMEM = "AT+SAPBR=0,1";                          // Disconnect GPRS

const char AT_CMD_HTTPINIT[] PROGMEM = "AT+HTTPINIT";                         // Init HTTP connection
const char AT_CMD_HTTPPARA_CID[] PROGMEM = "AT+HTTPPARA=\"CID\",1";           // Connect HTTP through GPRS bearer
const char AT_CMD_HTTPPARA_URL[] PROGMEM = "AT+HTTPPARA=\"URL\",";            // Define the URL to connect in HTTP
const char AT_CMD_HTTPPARA_USERDATA[] PROGMEM = "AT+HTTPPARA=\"USERDATA\",";  // Define the header(s)
const char AT_CMD_HTTPPARA_CONTENT[] PROGMEM = "AT+HTTPPARA=\"CONTENT\",";    // Define the content type for the HTTP POST
const char AT_CMD_HTTPPARA_REDIR[] PROGMEM = "AT+HTTPPARA=\"REDIR\",1";       // Enable HTTP redirection
const char AT_CMD_HTTPSSL_Y[] PROGMEM = "AT+HTTPSSL=1";                       // Enable SSL for HTTP connection
const char AT_CMD_HTTPSSL_N[] PROGMEM = "AT+HTTPSSL=0";                       // Disable SSL for HTTP connection
const char AT_CMD_HTTPACTION0[] PROGMEM = "AT+HTTPACTION=0";                  // Launch HTTP GET action
const char AT_CMD_HTTPACTION1[] PROGMEM = "AT+HTTPACTION=1";                  // Launch HTTP POST action
const char AT_CMD_HTTPREAD[] PROGMEM = "AT+HTTPREAD";                         // Start reading HTTP return data
const char AT_CMD_HTTPTERM[] PROGMEM = "AT+HTTPTERM";                         // Terminate HTTP connection

const char AT_RSP_OK[] PROGMEM = "OK";                                        // Expected answer OK
const char AT_RSP_DOWNLOAD[] PROGMEM = "DOWNLOAD";                            // Expected answer DOWNLOAD
const char AT_RSP_HTTPREAD[] PROGMEM = "+HTTPREAD: ";                         // Expected answer HTTPREAD
const char AT_RSP_SAPBR[] PROGMEM = "+SAPBR: 1,1";                            // Expected answer SAPBR: 1,1
