// The scaling line graph function
void drawGraph (
  const int16_t& a_xStartPos, const int16_t& a_yStartPos, const int16_t& a_graphHeight,
  const int16_t& a_graphWidth, const int16_t& a_numYLabels, uint16_t** a_data, const bool* a_compressed, 
  const int16_t a_numArrays, const int16_t& a_arrayNumber, const int16_t& a_percision, const uint16_t* a_colors)
{
  float minArrayVal = __FLT_MAX__, maxArrayVal = __FLT_MIN__; 
  tft.setFontScale(1);
  tft.setFont(&myriadPro_32px_Regular);
  tft.setTextColor(RA8875_BLACK, user::backgroundColor); 

  // work out the greatest number to display on the y axis.
  if (a_arrayNumber == 0) {
    minArrayVal = 0;
    maxArrayVal = 0;
  }
  else {
    for (int16_t i = 0; i < a_numArrays; i++) {
      for (int16_t n = 0; n < a_arrayNumber; n++) {
        if (uncompressShort(a_data[i][n], a_compressed[i]) > maxArrayVal)
        maxArrayVal = uncompressShort(a_data[i][n], a_compressed[i]);
        if (uncompressShort(a_data[i][n], a_compressed[i]) < minArrayVal)
        minArrayVal = uncompressShort(a_data[i][n], a_compressed[i]);
      }
    }
  }

  if (device::globalDebug) {  
    // Serial.print(F("compressed: ")); Serial.println(a_compressed[0]); 
    // Serial.print(F("minArrayVal: ")); Serial.println(minArrayVal, 2);
    // Serial.print(F("minArrayVal: ")); Serial.println(minArrayVal, 2);
  }

  int16_t maxGraphArrayValues = sensor::maxSensorArrayVals;
  bool redrawGraph = device::newGraphData;
  static float graphXstartPosition = a_xStartPos;
  static float xSpacing = 0;
  static float prevMinArrayVal = 0, prevMaxArrayVal = 0;
  if (display::refreshPage || hasChanged(minArrayVal, prevMinArrayVal, 2) || hasChanged(maxArrayVal, prevMaxArrayVal, 2)) {
    tft.fillRect(a_xStartPos, a_yStartPos, a_graphWidth, tft.height() - a_yStartPos, user::backgroundColor); //
    // work out the x position to start the graph at, based on the width on the y axis numbers. while drawing the draw y line and numbers   
    float yIncrement = 0;
    if (!isEqual(maxArrayVal,  minArrayVal, 0.01))
      yIncrement = (maxArrayVal - minArrayVal) / a_numYLabels;
    else
      yIncrement = maxArrayVal / a_numYLabels;
    float yLablePosition = a_yStartPos - 6;
    float tempYmax = maxArrayVal;
    graphXstartPosition = a_xStartPos;

    for (int16_t i = 0; i < a_numYLabels + 1; i++) {
      tft.setCursor(a_xStartPos, yLablePosition);
      tft.print(tempYmax, a_percision);
      yLablePosition += (a_graphHeight - 10) / a_numYLabels;
      tempYmax -= yIncrement;
      if (minArrayVal > 0 && tempYmax < 0)
        tempYmax = abs(tempYmax); // -0.00 is shown due to rounding issues close to 0
      if (tft.getFontX() > graphXstartPosition)
        graphXstartPosition = tft.getFontX();
    }   
    tft.drawLine(graphXstartPosition + 1, a_yStartPos + a_graphHeight, graphXstartPosition + 1, a_yStartPos, RA8875_BLACK);

    // draw x line and numbers
    float xLablePosition = graphXstartPosition + 1;
    xSpacing = ((float)(a_graphWidth - (graphXstartPosition - a_xStartPos)) / maxGraphArrayValues);
    float lableSpacing = (float)(a_graphWidth - (graphXstartPosition - a_xStartPos)) / (maxGraphArrayValues + 1);
    for (int16_t i = 0; i < maxGraphArrayValues + 1; i++) {
      tft.drawFastVLine(xLablePosition, a_yStartPos + a_graphHeight, 11, RA8875_BLACK);
      xLablePosition += lableSpacing;
    }
    tft.drawLine(graphXstartPosition + 2, a_yStartPos + a_graphHeight, xLablePosition - lableSpacing, a_yStartPos + a_graphHeight, RA8875_BLACK);
    redrawGraph = true;
    prevMinArrayVal = minArrayVal;
    prevMaxArrayVal = maxArrayVal;
  }
  else if (redrawGraph) {
    tft.fillRect(graphXstartPosition + 2, a_yStartPos, a_graphWidth - (graphXstartPosition - a_xStartPos), a_graphHeight, user::backgroundColor);
  }

  // draw line graph
  if (redrawGraph) { // display::refreshPage || 
    if (a_arrayNumber >= 2) {
      for (int16_t i = 0; i < a_numArrays; i++) {
        float xGap = graphXstartPosition + 2;
        float plotYstartPosition = 0, plotYendPosition = 0;
        for (int16_t n = 0; n < a_arrayNumber - 1; n++) {
          plotYstartPosition = mapFloat(uncompressShort(a_data[i][n], a_compressed[i]), minArrayVal, maxArrayVal, a_yStartPos + a_graphHeight - 1, a_yStartPos);
          plotYendPosition = mapFloat(uncompressShort(a_data[i][n+1], a_compressed[i]), minArrayVal, maxArrayVal, a_yStartPos + a_graphHeight - 1, a_yStartPos);
          tft.drawLine(xGap, plotYstartPosition, xGap + xSpacing, plotYendPosition, a_colors[i]);
          xGap += xSpacing;
        }
      }
    }
  }

}
