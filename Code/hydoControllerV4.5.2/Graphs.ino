// The scaling line graph function
void drawGraph (
  const int& a_xStartPos, const int& a_yStartPos, const int& a_graphHeight,
  const int& a_graphWidth, const byte& a_numYLabels, const bool& a_displayQuadrant,
  const float* a_data, const byte& a_arrayNumber, const byte& a_percision)
{
  float yMax = 0;
  tft.setFontScale(1);
  tft.setFont(&myriadPro_32px_Regular);
  tft.setTextColor(RA8875_BLACK);
  // work out the greatest number to display on the y axis.
  for (byte i = 0; i < a_arrayNumber; i++) {
    if (a_data[i] > yMax)
      yMax = a_data[i];
  }
  // work out the x position to start the graph at, based on the width on the y axis numbers.
  // while drawing the draw y line and numbers
  int graphXstartPosition = 0;
  float yIncrement = yMax / a_numYLabels;
  int YLablePosition = a_yStartPos - 6;
  float tempYmax = yMax;
  const byte yLableGap = (a_graphHeight - 10) / a_numYLabels;
  for (byte i = 0; i < a_numYLabels; i++) {
    if (i >= a_numYLabels)
      tft.setCursor(a_xStartPos - 14, YLablePosition);
    else
      tft.setCursor(a_xStartPos, YLablePosition);
    tft.print(tempYmax, a_percision);
    YLablePosition += yLableGap; // 33
    tempYmax -= yIncrement;
    if (tft.getFontX() > graphXstartPosition)
      graphXstartPosition = tft.getFontX();
  }
  tft.setCursor(a_xStartPos, YLablePosition);
  tft.print(0);
  tft.drawLine(graphXstartPosition + 1, a_yStartPos + a_graphHeight, graphXstartPosition + 1, a_yStartPos, RA8875_BLACK);
  // draw x line and numbers
  int XSpacing = (a_graphWidth - graphXstartPosition) / device::maxGraphArrayValues;
  int xLablePosition = graphXstartPosition + 2;
  for (byte i = 1 ; i < a_arrayNumber + 1; i++) {
    tft.setCursor(xLablePosition, a_yStartPos + a_graphHeight - 1);
    tft.print(i);
    xLablePosition += XSpacing;
  }
  int lineEnd = tft.getFontX();
  tft.drawLine(graphXstartPosition + 2, a_yStartPos + a_graphHeight, lineEnd - 4, a_yStartPos + a_graphHeight, RA8875_BLACK);
  // draw line graph
  float xGap = a_graphHeight / yMax;
  int yGap = graphXstartPosition + 10;
  float plotXstartPosition = 0, plotXendPosition = 0;
  if (a_arrayNumber >= 2) {
    for (byte i = 0; i < a_arrayNumber - 1; i++) {
      plotXstartPosition = a_yStartPos + a_graphHeight - (xGap * a_data[i]);
      plotXendPosition = a_yStartPos + a_graphHeight - (xGap * a_data[i + 1]);
      tft.drawLine(yGap, plotXstartPosition, yGap + XSpacing, plotXendPosition, RA8875_BLACK);
      if (a_displayQuadrant)
        tft.fillCircle(yGap, plotXstartPosition, 4, display::RA8875_SEABLUE);
      yGap += XSpacing;
    }
    // Very end quadrant to display
    if (a_displayQuadrant && a_arrayNumber >= 2)
      tft.fillCircle(yGap, plotXendPosition, 4, display::RA8875_SEABLUE);
  }
}
