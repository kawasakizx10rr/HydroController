const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<style>
body {
margin:0px;
padding:0px;
background-color:#404040;
}
p {font-family:"Verdana", Geneva, sans-serif;}
#wrapper {
margin:0px;
padding:0px;
width:200px;
height:200px;
}
#content {
position:absolute;
width:50%;
height:60%;
z-index:15;
top:20%;
left:25%;
background:#040404;
border-radius:0.5vw;
box-shadow:inset 0 0 10px grey;
}
#tftDisplay {
width:80%;
height:70%;
margin:7% 10% 0% 10%;
background:#33ccff;
}
#topMenu {
box-sizing:border-box;
width:100.2%;
height:14%;
float:left;
border-style:solid;
border-width:2px;
}
#leftMenu {
width:8.8%;
height:86.4%;
float:left;
box-sizing:border-box;
border-style:solid;
border-width:0px 2px 2px 2px;
}
#topbar {
height:5%;
}
#homepageblock1 {
height:50%;
}
.slideIcon {
width:0.7vw;
height:0.7vw;
}
.ptext {font-size:0.7vw;}
.p1text {font-size:0.8vw;}
.p2text {font-size:0.9vw;}
.p3text {font-size:1vw;}
.p4text {font-size:2.1vw;}
.p4Ltext {font-size:2.9vw;}
.p5text {font-size:3.5vw;}
#homePageMinTargetValue {margin-top:8%;}
#homePageMaxTargetValue {margin-top:8%;}
#pageArea {
width:90.8%;
height:85%;
float:left;
}
.topMenuButtonDiv {
width:11.11111%;
height:100%;
float:left;
cursor:pointer;
}
.leftMenuButtonDiv {
width:100%;
height:20%;
float:left;
cursor:pointer;
}
.bottomSection {
margin-top:0.5%;
width:100%;
height:17%;
}
.bottomLeft {
width:15%;
height:100%;
float:left;
}
.bottomMiddle {
width:50%;
height:100%;
float:left;
}
.bottomRight {
width:25%;
height:100%;
float:left;
}
#clock {
font-weight:bold;
margin:0px;
font-family:"Verdana", Geneva, sans-serif;
}
#lable {
color:white;
text-align:center;
padding:0px;
margin-top:6%;
font-weight:bold;
font-size:1.4vw;
font-family:"Verdana", Geneva, sans-serif;
}
#led {
position:absolute;
width:2.7%;
height:3.7%;
top:90%;
float:left;
}
.led-blue {
background-color:#6699ff;
box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #006 0 -1px 9px, #0066ff 0 2px 4px;
border-radius:4px;
}
.led-yellow {
background-color:#ff0;
border-radius:4px;
box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #808002 0 -1px 9px, #ff0 0 2px 2px;
-webkit-animation:blinkYellow 1.5s infinite;
-moz-animation:blinkYellow 1.5s infinite;
-ms-animation:blinkYellow 1.5s infinite;
-o-animation:blinkYellow 1.5s infinite;
animation:blinkYellow 1.5s infinite;
}
.led-red {
background-color:#f00;
border-radius:4px;
box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #441313 0 -1px 9px, rgba(255, 0, 0, 0.5) 0 2px 2px;
-webkit-animation:blinkRed 1s infinite;
-moz-animation:blinkRed 1s infinite;
-ms-animation:blinkRed 1s infinite;
-o-animation:blinkRed 1s infinite;
animation:blinkRed 1s infinite;
}
#leftSlideArrow, rightSlideArrow {
width:2.6%;
height:8%;
position:absolute;
top:46%;
left:18%;
opacity:0.1;
z-index:20;
}
#leftSlideArrow:hover, rightSlideArrow:hover {
opacity:1;
}
#rightSlideArrow {
width:2.6%;
height:8%;
position:absolute;
top:46%;
right:10.6%;
opacity:0.1;
z-index:20;
}
#rightSlideArrow:hover {
opacity:1;
}
.settingbutton {
margin-top:14px;
margin-right:20%;
width:35%;
height:60%;
}
@-webkit-keyframes blinkYellow {
from {background-color:#ff0;}
50% {background-color:#aa0;box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #808002 0 -1px 9px, #ff0 0 2px 0;}
to {background-color:#ff0;}
}
@-moz-keyframes blinkYellow {
from {background-color:#ff0;}
50% {background-color:#aa0;box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #808002 0 -1px 9px, #ff0 0 2px 0;}
to {background-color:#ff0;}
}
@-ms-keyframes blinkYellow {
from {background-color:#ff0;}
50% {background-color:#aa0;box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #808002 0 -1px 9px, #ff0 0 2px 0;}
to {background-color:#ff0;}
}
@-o-keyframes blinkYellow {
from {background-color:#ff0;}
50% {background-color:#aa0;box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #808002 0 -1px 9px, #ff0 0 2px 0;}
to {background-color:#ff0;}
}
@keyframes blinkYellow {
from {background-color:#ff0;}
50% {background-color:#aa0;box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #808002 0 -1px 9px, #ff0 0 2px 0;}
to {background-color:#ff0;}
}
@-webkit-keyframes blinkRed {
from {background-color:#f00;}
50% {background-color:#a00;box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #441313 0 -1px 9px, rgba(255, 0, 0, 0.5) 0 2px 0;}
to {background-color:#f00;}
}
@-moz-keyframes blinkRed {
from {background-color:#f00;}
50% {background-color:#a00;box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #441313 0 -1px 9px, rgba(255, 0, 0, 0.5) 0 2px 0;}
to {background-color:#f00;}
}
@-ms-keyframes blinkRed {
from {background-color:#f00}
50% {background-color:#a00;box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #441313 0 -1px 9px, rgba(255, 0, 0, 0.5) 0 2px 0}
to {background-color:#f00;}
}
@-o-keyframes blinkRed {
from {background-color:#f00;}
50% {background-color:#a00;box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 1px 1px, inset #441313 0 -1px 9px, rgba(255, 0, 0, 0.5) 0 2px 0;}
to {background-color:#f00;}
}
@keyframes blinkRed {
from {background-color:#f00;}
50% {background-color:#a00;box-shadow:rgba(0, 0, 0, 0.2) 0 -1px 7px 1px, inset #441313 0 -1px 9px, rgba(255, 0, 0, 0.5) 0 2px 0;}
to {background-color:#f00;}
}
.calanderDayLeft {
background-color:white;
width:14%;
height:18%;
display:block;
float:left;
box-sizing:border-box;
-moz-box-sizing:border-box;
-webkit-box-sizing:border-box;
border-top:2px solid #000;
border-left:2px solid #000;
outline-offset:0px;
text-align:center;
cursor:pointer;
font-weight:bold;
line-height:18%;
}
.calanderDayRight {
background-color:white;
width:14%;
height:18%;
display:block;
float:left;
box-sizing:border-box;
-moz-box-sizing:border-box;
-webkit-box-sizing:border-box;
border-top:2px solid #000;
border-left:2px solid #000;
border-right:2px solid #000;
text-align:center;
cursor:pointer;
font-weight:bold;
line-height:18%;
}
.calanderDayBottom {
background-color:white;
width:14%;
height:18%;
display:block;
float:left;
box-sizing:border-box;
-moz-box-sizing:border-box;
-webkit-box-sizing:border-box;
border-top:2px solid #000;
border-left:2px solid #000;
border-bottom:2px solid #000;
text-align:center;
cursor:pointer;
font-weight:bold;
line-height:18%;
}
.calanderDayEnd {
background-color:white;
width:14%;
height:18%;
display:block;
float:left;
box-sizing:border-box;
-moz-box-sizing:border-box;
-webkit-box-sizing:border-box;
border:2px solid #000;
text-align:center;
cursor:pointer;
font-weight:bold;
line-height:18%;
}
.rightSlideArrowIcon {
-webkit-transform: scaleX(-1);
transform: scaleX(-1);
}
.ldbutt {
-webkit-transform: scaleY(-1);
transform: scaleY(-1);
height:80%;
float:left;
cursor:pointer;
margin-top:2%;
}
.lubutt {
height:80%;
float:right;
cursor:pointer;
margin-top:2%;
}
.leftToggle {
display:block;width:42%;height:100%;float:left;background:red;border-radius:0.4vw 0vw 0vw 0.4vw;border:0.05vw solid;cursor:pointer;
}
.rightToggle {
display:block;width:42%;height:100%;float:left;background:lightgrey;border-radius:0vw 0.4vw 0.4vw 0vw;border:0.05vw solid;cursor:pointer;
}
::-webkit-scrollbar {width: 0.6vw;}
::-webkit-scrollbar-track {background: #99ccff;}
::-webkit-scrollbar-thumb {background: #1a8cff;}
::-webkit-scrollbar-thumb:hover {background: #0073e6;}
</style>
<meta name="viewport" content="width=device-width, initial-scale=1.0" />
<title>Hydro Controller v3.4 Web GUI</title>
<script type="text/JavaScript">
// LOCAL GUI VARIABLES
// USER
// sensor arrays
let ecSenorArray = [];
let tdsSenorArray = [];
let phSenorArray = [];
let co2SenorArray = [];
let waterTemperatureSenorArray = [];
let waterLevelSenorArray = [];
let airTemperatureSenorArray = [];
let airHumiditySenorArray = [];
let fanOneSpeedArray = [];
let fanTwoSpeedArray = [];
let autoFillDays = [];
let arrayCount = 0;
// profile names
var profileOneName = "-";
var profileTwoName = "-";
var profileThreeName = "-";
var profileFourName = "-";
var profileFiveName = "-";
// Light page
let onTimeHour = 0;
let onTimeMin = 0;
let offTimeHour = 0;
let offTimeMin = 0;
let lightMode = 0;
// sensors
let ecSensor = 0;
let tdsSensor = 0;
let phSensor = 0;
let co2Sensor = 0;
let waterTemperatureSensor = 0;
let waterLevelSensor = 0;
let airTemperatureSensor = 0;
let airHumiditySensor = 0;
// fans
let fanOneSpeed = 0;
let fantwoSpeed = 0;
let fanOneMinSpeed = 0;
let fanOneMaxSpeed = 0;
let fanTwoMinSpeed = 0;
let fanTwoMaxSpeed = 0;
var fansControlTemp = true;
var fansControlHum = true;
var fanOneFixedSpeed = false;
var fanTwoFixedSpeed = false;
// user targets
let targetMinAirTemp = 0;
let targetMaxAirTemp = 0;
let airTempOffset = 0;
let targetMinHumidity = 0;
let targetMaxHumidity = 0;
let humidityOffset = 0;
let targetMinTds = 0;
let targetMaxTds = 0;
let tdsOffset = 0;
let targetCo2 = 0;
let co2Offset = 0;
let roomWidth = 0;
let roomLength = 0;
let roomHeight = 0;
let co2Flowrate = 0;
let co2CheckTimeMinute = 0;
let co2CheckTimeHour = 0;
let disableFansTimer = 0;
var manGasTimeEnabled = false;
let manualGasDurationTime = 0;
var co2ControlDisabled = false;
let targetMinPh = 0;
let targetMaxPh = 0;
let phOffset = 0;
let targetMinEc = 0;
let targetMaxEc = 0;
let ecOffset = 0;
let targetMinWaterTemp = 0;
let targetMaxWaterTemp = 0;
let waterTempOffset = 0;
let targetMinWaterHeight = 0;
let targetMaxWaterHeight = 0;
let waterHeightOffset = 0;
// max mins
let minPh = 0;
let maxPh = 0;
let minCo2 = 0;
let maxCo2 = 0;
let minEc = 0;
let maxEc = 0;
let minWaterTemp = 0;
let maxWaterTemp = 0;
let minWaterLevel = 0;
let maxWaterLevel = 0;
let minTds = 0;
let maxTds = 0;
let minAirTemp = 0;
let maxAirTemp = 0;
let minHumidity = 0;
let maxHumidity = 0;
let minFanOneSpeed = 0;
let maxFanOneSpeed = 0;
let minFanTwoSpeed = 0;
let maxFanTwoSpeed = 0;
// dosers
let numDosers = 0;
let doserOneMode = 0;// EC / PH + / Off
let doserTwoMode = 0;// EC / PH + / Off
let doserThreeMode = 0;// EC / PH + / Off
let doserFourMode = 0; // EC / PH - / Off
let doserFiveMode = 0;// EC / PH + / Off
let doserSixMode = 0; // EC / PH - / Off
let doserOneMills = 0;
let doserTwoMills = 0;
let doserThreeMills = 0;
let doserFourMills = 0;
let doserFiveMills = 0;
let doserSixMills = 0;
//let doserOneSpeed = 100;
//let doserTwoSpeed = 100;
//let doserThreeSpeed = 100;
//let doserFourSpeed = 100;
//let doserFiveSpeed = 100;
//let doserSixSpeed = 100;
let dosingInt = 0;
let swapInterval = 0;
let refillDoserOneMills = 0;
let refillDoserTwoMills = 0;
let refillDoserThreeMills = 0;
let refillDoserFourMills = 0;
let refillDoserFiveMills = 0;
let refillDoserSixMills = 0;
let autoFillHour = 0;
let autoFillMinute = 0;
// warnings
let tdsErrorMargin = 0;
let ecErrorMargin = 0;
let phErrorMargin = 0;
let co2ErrorMargin = 0;
let waterTempErrorMargin = 0;
let waterHeightErrorMargin = 0;
let airTempErrorMargin = 0;
let humidityErrorMargin = 0;
var disablePpmWarn = false;
var disableEcWarn = false;
var disableCo2Warn = false;
var disablePhWarn = false;
var disableWaterTempWarn = false;
var disableWaterHeightWarn = false;
var disableAirTempWarn = false;
var disableHumidityWarn = false;
// envriomental control
var disableDrainAndRefill = false;
// DEVICE
var convertToF = false;
var convertToInchs = false;
var convertToTds = false;
// system
var ledTimer;
const warningState = {
NO_WARNING:0,
MINOR_WARNING:1,
MAJOR_WARNING:2,
}
const doserModes = {
DOSER_OFF:0,
EC_DOSER:1,
PH_DOSER:2,
}
var previousErrorState = warningState.NO_WARNING;
var ppmErrorState = warningState.NO_WARNING;
var ecErrorState = warningState.NO_WARNING;
var phErrorState = warningState.NO_WARNING;
var co2ErrorState = warningState.NO_WARNING;
var waterTempErrorState = warningState.NO_WARNING;
var waterLevelErrorState = warningState.NO_WARNING;
var airTempErrorState = warningState.NO_WARNING;
var humidityErrorState = warningState.NO_WARNING;
var userProfile = 0;

let screenWidth = window.innerWidth;
let screenHeight = window.innerHeight;
let homePageScrollPosition = 0;
let p2SPos = 0;
let maxMinPageScrollPosition = 0;
let co2PageScrollPosition = 0;
let waterPageScrollPosition = 0;
let doserPageScrollPosition = 0;
let fanPageScrollPosition = 0;
let warningsPageScrollPosition = 0;
let waterPageDoserScrollPosition = 0;
let loadProfileNum = 0;
let saveProfileNum = 0;
let connectionTime = 0;


(function () {
  var width = window.innerWidth;
  window.addEventListener('resize', function () {
   if (window.innerWidth !== width) {
     window.location.reload(true);
   }
  });
})();

function setDisplaySize() {     
  var link = document.querySelector("link[rel~='icon']");
  if (!link) {
      link = document.createElement('link');
      link.rel = 'icon';
      document.getElementsByTagName('head')[0].appendChild(link);
  }
  link.href = document.getElementById("ico").src;

  let box = document.getElementById("homePageGraph");
  var tempCanvas = document.getElementById("graphCanvas");
  tempCanvas.width = box.offsetWidth;
  tempCanvas.height = box.offsetHeight;   

  document.getElementById("button1_Img").src=document.getElementById("home_blue").src;
  document.getElementById("button2_Img").src=document.getElementById("graph_blue").src;
  document.getElementById("button3_Img").src=document.getElementById("maxMins_blue").src;
  document.getElementById("button4_Img").src=document.getElementById("presets_blue").src;
  document.getElementById("button5_Img").src=document.getElementById("cogs_blue").src;
  document.getElementById("button6_Img").src=document.getElementById("ec_blue").src;
  document.getElementById("button7_Img").src=document.getElementById("ph_blue").src;
  document.getElementById("button8_Img").src=document.getElementById("co2_blue").src;
  document.getElementById("button9_Img").src=document.getElementById("waterTemp_blue").src;
  document.getElementById("button10_Img").src=document.getElementById("doser_blue").src;
  document.getElementById("button11_Img").src=document.getElementById("light_blue").src;
  document.getElementById("button12_Img").src=document.getElementById("fan_blue").src;
  document.getElementById("button13_Img").src=document.getElementById("warning_blue").src;
  
  var ldbutt = document.getElementsByClassName('ldbutt');
  var lubutt = document.getElementsByClassName('lubutt');
  for (var i = 0; i < ldbutt.length; i++) 
    ldbutt[i].src = document.getElementById("largeUpButton").src
  for (var i = 0; i < lubutt.length; i++) 
    lubutt[i].src = document.getElementById("largeUpButton").src
    
  var lsa = document.getElementsByClassName('leftSlideArrowIcon');
  var rsa = document.getElementsByClassName('rightSlideArrowIcon');
  for (var i = 0; i < lsa.length; i++) 
    lsa[i].src = document.getElementById("leftScrollArrow").src
  for (var i = 0; i < rsa.length; i++) 
    rsa[i].src = document.getElementById("leftScrollArrow").src
    
  var dl = document.getElementsByClassName('doserL');
  for (var i = 0; i < dl.length; i++) 
    dl[i].src = document.getElementById("doser_blue").src 

  document.getElementById("wrapper").style.width = screenWidth + "px";
  document.getElementById("wrapper").style.height = screenHeight + "px";
  
  if(screenWidth<1000) {  
    var el = document.getElementById("content");
    el.style.width="90%";
    el.style.height="90%";
    el.style.top="5%";
    el.style.left="5%";
  }
}

function setGraph2Size() {
  let box2 = document.getElementById("graphPageGraph");
  var tempCanvas2 = document.getElementById("graphCanvas2");
  tempCanvas2.width = box2.offsetWidth;
  tempCanvas2.height = box2.offsetHeight;
}

function startTime() {
  var today = new Date();
  var h = today.getHours();
  var m = today.getMinutes();
  var s = today.getSeconds();
  m = checkTime(m);
  s = checkTime(s);
  var l = " noon";
  if (h < 12) l = " am"
  else if (h > 12) l = " pm"
  document.getElementById('clockText').innerHTML = h + ":" + m + ":" + s + l;
  var t = setTimeout(startTime, 500);
}

function checkTime(i) {
  if (i < 10) 
    i = "0" + i
  return i;
}


</script>
</head>
<body onload="setDisplaySize(), startTime(), homePageSlider(0), setPage('1', true)" onresize="setDisplaySize(),homePageSlider(0)">

<img id="ico" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAA
Bzenr0AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAAFi
QAABYkAZsVxhQAAADISURBVFhH7ZYBDoMgDEXpwiE83Q7n6bwDCcxqvxLFwZ
yjM+ElTVss4dOGRAohGE0e4tVoApqA5RUQUdXnMJ5L7NsI0iPoBwku5tlJUD
ICFF/lD7hBB5gPbzXBNbA3qHeAg8k4XKwfVo94+/3LdZzbOnDDDpzJYdE6zj
1+htsuIMctSvMM6iNIC6gABKRH8KsOJMh2YFyXaI6995KVwXustcY5Jyszux
GAWAgX5XL2Z/YA9R+SnQAoLPVMaS18zP91oDZNgLIAY14RFUaQaq/6SwAAAA
BJRU5ErkJggg=="alt="icon"style="display:none;"/>

<img id="co2_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAMAAA
C5zwKfAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAIrUExURQAm/Q
c2/wEl/wIk/wMl/wQk/wAn/gEq/wM5/QEm+wIj/wAn+AEm+QMl+AIl+QAp9Q
Ij/AIl/wMq8wAm9wUl/wIf/wAl/gMn/wQl/gUj/QMm/gEi+QQl/AUo/AIm/w
Ao+wAq9AAt/wAq/QMo/QAp+gAl+gAq/wAn/wAn+wAn/QMg/wQm/wUm/wAh+A
Ai9AAo+QMm9AMm+gAo8Agn+wQu/gAq8gIn/AAr+gAi+AU2/AAl/wIr9wEr+Q
As/wcg+QAq+wYj/QEg9AMk/wAm/wAo+gAo/QEr8wAm+gEl/QEo8gUl8gAr+A
Io9wIl/QIl+wAo9AAp9AAo/AAq+AAj/QAo/wMl9gAk/AMj/wcq/wkt8wEj/g
Qq9wAt7AQm+gMn9wMx9gAm+QQn/QEk+gAh9AAo8wIq/gAn9AAp+AYp/wAn8w
Mr/wAo9gAl+QAo/gQo/wAi/QAh+wA5/AAj9wQn/wMi+Qko/wgw/wk3/gYl+g
Ei+w0n+gAr7QAm9QEo/wAj+wIm/gAk+AAm/AAm/gAp8gAp/wAj+gAn9QAg/g
Ep/wAh9wAn+QAh/AAl8wEk+Aco/wMk+wMk/QEn+AAp+QUp/wAk9gUs/wAl9g
At/AEp+wAh9QAv9QM3/QIp/wAs9QAp/QAg9gQq+wMp+AYs/QAk9QAq9wMt9Q
Ap6QAu/QAt6gEf+QAp6wIo+QAi+gAp/AAl/AQl/wAt5Qso/wAf/QAh/QAk/Q
cv/wAi9wAo+AAAABbK0yoAAAC5dFJOU/////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////8AQAvpHgAAAAlwSFlzAAAOww
AADsMBx2+oZAAAA1hJREFUWEftlmdbE1EQhY8G0SDYxYJdMKjYBRG7iMaGYu
+9994LYkexY2/Yey/8PGfmzrYkqE82H/f9cO/MOXPPZvcJG1CfYoJA/wSB/g
kC/RME+icI9I8nEB5UNKjGqNIAjk2jjcwJNA7xqjohItKapNPatBktqifC9o
BwBtBcjhrU0LzMzKwWtHLZ8m+JliWHiFattXAOAW3QFu3aZ3fo2KlV55wuXd
1mHOpIQjdZhe497DPSdwR69uotJTJiHogXY9BALk8hr08EyO8rtTiS168/UD
CAioFtgUEYjCG2HYcVOBQYRlPD+bwiDlmFRSoIdEGDseMQnZ5zBHIjcXOkjC
g2DlvAyGx0KLHaBGjgKGB0wiEWxwBjbYv6zHHybVAhBg0ExtNzTxg4YSIb7s
CCUkxyhBiswPQy1yEXwOQpiE51WcC06cj5Z2BjzksYOAMI0R1qL8P50cTDzP
8EhmIDwzMTDzNJBZb7CqQ75m+U9iYwilnJB9In9FhUh9MxO/lARXtRKvg9ls
pAeYn5CIy7ZVegVAbp7cCSUkdzw6qgvShzbIW2ufPkW0kYW+WMKOY3FJhtTz
NUL2CBFdkVeheIrTMLF2FxA4HlS3hee1Hot0AUXjOXSgMs8waaNwA3Xkhc7r
WoXqGKbMJKYJU3sFx0bryIzGjPymrrfSiblWBVTktw44XVNR4LWLuOBVJ0M6
qWZmHWl2xIkCiWYAsbN21WwSNrbRZiS1546/8FbrMFs6qstVmI7Tt27sLuuE
gZVEy7R+qYl4NoTiD3a/bu229kC3HYOmB65mDxId4OH4nNO3qMZClVAI6fOF
kJnKqqquLutCsQZ3hxcbbynCeQJPr3RKNklWM4H7mAi6GFoVB1NbfGYStyiX
ubspifKKNaSWYj8TKLNVeuXiu/Xlh4wx6w55UTV27yph7D7S3n+rqLXMsL0t
IqKnhXw514+07u3XtcqMNwe991ed3FePCwqMi8mpDvPsTCo8dPVj+te/b8BT
eqM9y+dF9dd4FNBxUNqjGqGIBXqPNcXPck4Qt4/x78BXLe6zfaGHwFct7bGl
4NoomTJMC798CH2u4fP136nPUlFYHE12/ffxz/+SvyOyWf0Ito4qSQINA/Qa
B/gkD/BIH+SXFgff0fxIxrxTLq0eQAAAAASUVORK5CYII="alt="co2_blue"style="display:none;"/>

<img id="cogs_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAYAAA
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" alt="cogs_blue"style="display:none;"/>

<img id="doser_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAYAAA
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"alt="doser_blue"style="display:none;"/>

<img id="ec_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAYAAA
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
8FZv8FChgHppKqCugAAAAASUVORK5CYII="alt="ec_blue"style="display:none;"/>

<img id="fan_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAYAAA
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"alt="fan_blue"style="display:none;"/>

<img id="graph_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAYAAA
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
BSoBDZkKrLSGUvpJI6ylPO+z0t0mZw8QYt3xFzqyO90BfoNDRvp1xaHpDynj
ExsMvY3kPI580fM7dCWaR1kmieyK5351EGOvP61j7j9OfYoINskAJyvOEK0s
tkbXgh1958JcgTtv4gp295WzyMHBhcyuBUJiURNaTZktmIuomT2xuoOo6FI5
a3hwnhrnVGHph9/Goobw1be5h3PvH2eIo8Tojp+4aceFUbJZuxr2nFfhWMqZ
0Z6Ls3fb1bt05j3vxvfvJKkSds/oHO3PVu8ThC41Il9bRDJ5ZZrUqtk3VoT7
EVr1CIvWgo1T8/ztNKEVJuRRWXH3xeFrK8t0K+bFzTRyf2DHnC8w/2Aj7vwS
idWJ1OgA4hTek2IE3voIEBHvscQ7x3lLeG5xvweQrGnGaSR+ELxemq4JZBBx
IP0RFcO7TnyBP6D3oakzxPtc01xQvEm7xt5q2sQdw41fbS9/YkecKaTV0f7+
DzWpp3019rA/k8qFKnYQ4mqYx7mTzh2YNXh7F4hA2McYJR1hdUIo9ffa5WCk
92ZZPfNwwg1lfg+U+9a01a0qHaFpp8A4SsVauWNmFwlNCN5AV828b88qG3ey
PkuZx6oRkQb2nhAtNVbdmqkdsUjE32tgOM9YnQ1HlBp7HctNTXw3/vVJr1Vc
HY6MuePYj1Q/jDt5yly+Z9HqbXqzo+b/JeJG8dbEyIqvD8wd601MdHInlbwn
uXqB4REREREREREREREREREREREREvH2b/ARKQcZZmwTPjAAAAAElFTkSuQm
CC"alt="graph_blue"style="display:none;"/>

<img id="home_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAYAAA
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
SuQmCC"alt="home_blue"style="display:none;"/>

<img id="largeUpButton" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACgAAAAoCAYAAA
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
=="alt="largeUpButton"style="display:none;"/>

<img id="light_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAYAAA
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
SuQmCC"alt="light_blue"style="display:none;"/>

<img id="maxMins_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAYAAA
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"alt="maxMins_blue"style="display:none;"/>

<img id="ph_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAYAAA
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"alt="ph_blue"style="display:none;"/>

<img id="pm_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAMAAA
C5zwKfAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAALHUExURQAz4w
Ao+gAl/wAk/wEj/gAk/gAj/wAm/AAn+AAo+AAq+AAw6wA04wAp/AAn/wAm/w
Am/gAl/gAw6gAy4wAy6wMl/wQk/wMm/AIn/AAo/wAp+gIl/wYj/wIm/wAs/A
Am9wQm/wEh/wAx8QA06wIm/gAo/gEo/wAr7AAr6gEn/wMm/gMk/wAy6AAn/Q
An+wEl/wAn/AAo+wIl/QYk/gIj+gAn9QIk/wUj/QMl+AIl+QAm/QAp8wAy6Q
Ao+QAy6gAx6AEl/QAo/Acj/wUh/wEm+wAk+gAu7QAx5gAp/QUj/wUn/wAj+w
Ay7QQo/wAq/QAs/wAo/QAn/gAp/wAq/gAn+QIr/wAi+gAq8wA24wAx5AAk/Q
Ap9QAx4gAw5AAp9wAo9gMo+wAh/wEk/wAx5QAt6wAp/gMo/QMk/QAk9QAk+A
Ih/wAs+AAw6AMp/wAr+wAu4wAv4QAt7AAs9gIq/wAs9QA43gAi/QMq/wAk9w
Iq/gAj+QAq9gAm7gEk+AAn+gAu8AAw4wAx6gMj/wAi/wMn/wAp9gAw4QA16A
Ap+QAg/QAq8AA17AAy5AEp/QAi+QAw7gUl/wAl+QAz5wAy4gIp/wAj/gAj/A
Aq9AAz4QEp/wAp9AAz6AQl/gAp+AAz6gAv5wIl+wAu+gAw4AA03QEr/wAm8Q
Av5QAw5gAz7AA06AAo9wAy7AAt4gQi/wQi/gUm/wAq9wAv6gAp+wAw7AAl/A
Ak/AAv7gAv7AEi/QEi+QMk+wA35AAu6wAw5QAs+QEr+wAx4wAl+gAq/wAu/w
Aj/QA08gAv6AAy5QAu6AAx5wUo/wMi/wgk/wEi+wYp/wAk+QAi/AMr/wA24A
Ar9AAy4QAq/AAl9wUp/wAu7gA06QMm/wIk9QIe/QAk+wAm+wA05wAl+AIq+w
Ai/gAl+wMs/wAq+wMm+gAt8wAd/gAj9wA27AAz5AAAAMw18t8AAADtdFJOU/
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//////////////AOpBRAoAAAAJcEhZcwAAFiUAABYlAUlSJPAAAARhSURBVF
hH7ZT7WxRVGMfdMzBAxHlhkcWBNrNwKrado1RTjS7M1hohKuwmklASq2VUaq
VZtmUlS1KRRlGbW2TR/X6x+127WVnZPbvfr/NHdObMgU1xdx8anp6ep/n+sM
983+9zPnsu7zkTjHGWA7QvB2hfDtC+HKB9OUD7GgFOcCEhJ1dAgiAKefkFrr
x9Cnliad8iDMUlyOXmnqvQJeTmCMhVyn0KOBHKPOWTsFRRIVV69vN49598wB
Q4kIdUB0lQWTVpKpYP5gWmQw7FuKra48MTeSEFPAz7lXwFYwlj8PkInja95v
AjjkzNx636jjr6GABtxkxeoZo5AwME8hUN1/JKClgHOqGpHtRFz7HHhRSMFK
Volvd4Hhv1/hMk0tDQMDu3cQ4vGXMac+eyUQTqeGk3INAIzytqag5HTmye37
JArRSh9SSeL1QkCQMhbVhuP9kqndIq4zY2KgNQX9Rxqt4ZJYLaWV2jVgYK8h
Zb+cIQ/Te6IREJAJaYlSX0Q5K19EAWqb5OVHxay+lLy4T8M7rmYjjzLCs/mx
CiYYwRJuKy5SsMY8XyZQLByJwGHZcW2HGOZWvPPW/aSgE6o2jV+azgJpquqn
SSq+GCC4PBNWuCRRe1wGqcAciWHBsplF58iSyvLWi99DJmLwc/+NbV6FKkKy
iTaHd3lMSDsyI4w5IZsOcK7o31vb7olagpcNXVzPb5NUJUTSvG12yIi4iuXY
xvkP0l2YCxPu4NYyO0hYON1/Zfx9z1CkQB/HjghjhAhJ6H+RO/cYCNSgM0Oy
pxE/d0To1oExB/8mbmakGtIeZgQQK9/Baqch0kgR5V5hkO3sq9YWyOrhRqbg
vdbl1TemgEIUgOGUldCYsAYljRk8ZQMsuSE3dwT+/9nQIOz1Puupu5e3SNxt
B7r2HctwjheByj+9cbxgOx9ECrbYa4Nx4MPPTwI2FNeXQzs4+tw82KBo+bfb
6l7Imurie9T9HPxQk6io5LC4wNX7WnN5U98+xzxKu1Ps/8CxQ4XYEAG/niS1
7vy6+YX/XmDNMArbahd8BU/aqt2/Crkddg6+tW4Y2l9BkCLfYmc9slaTv7eC
vbHpK3VSzibe/sePe990uQtPOD9g+tnO4h3ULto48tO6xSc8kZgf2KJn8yf1
CRkevTzwR57ec8rydm7+HYLu65dmWaIeuoL7pFVBxs/3LHV1VfV8/+5lse09
wU6k01PlNfIksf6t99XzHZVfUDkiJNkZ0Vqa50MyAMjgJmW/ICAkiCHyUBRH
lgCw+p3EB0+sD2jAFoXfOf8vqxiH7u+CUn8iuPmAop0K/pyb0B6bj0wN88no
YpauT3qRt5wGUBtcQYgFYf/sH9nioEPeTX/v4aMWXdw9jIe7iH3BAKKf/gUH
qsmzBadVpI8dPH4U/uuTICzY6Kpd7D3TWcj5phlj7MBGT5mPdwHIHW85UeaO
V7A9L6vwL87+/hOAPHSw7QvhygfTlA+3KA9uUA7et/BzSMvwA+PEMqX+RjBg
AAAABJRU5ErkJggg=="alt="pm_blue"style="display:none;"/>

<img id="presets_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAYAAA
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
0AAAAASUVORK5CYII="alt="presets_blue"style="display:none;"/>

<img id="leftScrollArrow" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAB8AAAA8CAYAAA
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"alt="leftScrollArrow"style="display:none;"/>

<img id="slideIcon_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAYAAA
CNiR0NAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAAFi
UAABYlAUlSJPAAAAHfSURBVDhPtZRLbtwwEESLvxmNDRjIVXKDLHPiLH0DXy
O7wICBsT4kO9Uk9R15l9RAI4pqPXYXW8K/lmnnU/34+SZtuNPrr+9fPnd6Yw
t6D3fYLAg2AN7h9unanXPww8QME5uQx4gQCMpZJzAMA/ztiskLLrGCj9Ddxb
HEmBOQMrxCN7rHCKPZtse30GVwhI2wzM7BTFObWWVsx6wFd/5u2GdqyxX1+/
mjjYDJBfRpOIWpJPcYpx5X+DazqgA1uy7VlfpEiMl4IXSgV18pBCZESxz9fQ
/9UuGS4bf+qZyv2SBEeqfjuDhSJLo5G4kRRGNY/Rq3AFVWIrxlpiIY8pl3NT
xJXVAr0Y3rOnratAOK8xg1mKt24coNju1O7RyVZulMtcdIJsBgGsZyrdoBR9
0EBpe2Y7s4u5quVSW6ZNmPqixagWWM3UF2QG1iw+wYBhN5K9ZNyVxBs6FlrK
JMQRilhympr94ubm770MiFNqZyqE9qvsq2UrVs7p2OlvIf+nArMSMmQkauXG
H0qj2YdSGeLf9m2FYL8PhOWs/SN/EJUzlUpV2U2HT66s06voIp8SNBsqf5Wf
tTbbgYNvS+1FkPQNUR+icktZ+lcued4KWvH4sjTHUKnHUEzzoD/ScBfwHT4+
fAdbgQNAAAAABJRU5ErkJggg=="alt="slideIcon_blue"style="display:none;"/>

<img id="warning_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAMAAA
C5zwKfAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAGUExURQEq9A
AAAOxO3D8AAAACdFJOU/8A5bcwSgAAAAlwSFlzAAAOxAAADsQBlSsOGwAAAV
tJREFUWEftkVtuwzAMBOP7X7qVPYZFi0vRL7VoPT8Rl8tJgHymm3mF13mF1/
kjwk+Bd49Mb/ZljT8hxJc09lvYCiQh44W4FsgihgsxrZAGjBbi2SDXDBZiqW
EjyQntMyQs4DDCnjElbAdNtMewE3aMGaE/CoI1940wNiaEjE7gobdcO8LQ2B
fy3kcKueR2vjafhbnh0xVuTxsq1I5LIQyMPWH13sc+YsWdFGpjR1gP9fuokK
tAKI2x0ExmOCbkJhQqYyi0o52OCLnoCIVxhJD+duGPwjhASLvqi9k3Pi+kW7
dV4BofF9I0XZl4xqeF9GxTR44xI1xS3gtzUmDeyPb20GubzwrpNC2ZFZhXMs
IoDIU01I9hWiFtvojPAoXbhOybSyEU/Yxw2fDemNMC84IjZOxDXwnZnhCai1
bImIELX8juHDgKjwnZnAXLN/9YePufchuv8Dq/XThNX5bsExQxiqXwAAAAAE
lFTkSuQmCC"alt="warning_blue"style="display:none;"/>

<img id="waterTemp_blue" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAFAAAABQCAYAAA
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
5TQOTOAAAAAElFTkSuQmCC"alt="waterTemp_blue"style="display:none;"/>

<div id="wrapper">
<div style="display:block;float:right;text-align:right;">
  <p class="p1text" style="margin-right:2px;color:yellow;display:none;vertical-align:bottom;" id="deviceTitleWarning">
    <b>&#9888;</b>
  </p>
  <p class="ptext" style="margin-right:18px;color:red;display:inline;" id="deviceStatusTitle">
    <b></b>
  </p>
</div>

<div id="content">
  <div id="tftDisplay">
    <div id="topMenu">
      <div class="topMenuButtonDiv" id="button6" onclick='setPage("6", true)' style="margin-right:1.68875%;">
        <img id="button6_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
      <div class="topMenuButtonDiv" id="button7" onclick='setPage("7", true)' style="margin-right:2.5875%;">
        <img id="button7_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
      <div class="topMenuButtonDiv" id="button8" onclick='setPage("8", true)' style="margin-right:2.4875%;">
        <img id="button8_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
      <div class="topMenuButtonDiv" id="button9" onclick='setPage("9", true)' style="margin-right:1.38875%;">
        <img id="button9_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
      <div class="topMenuButtonDiv" id="button10" onclick='setPage("10", true)' style="margin-right:0.18875%;">
        <img id="button10_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
      <div class="topMenuButtonDiv" id="button11" onclick='setPage("11", true)' style="margin-right:0.3%;">
        <img id="button11_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
      <div class="topMenuButtonDiv" id="button12" onclick='setPage("12", true)' style="margin-right:1.58875%;">
        <img id="button12_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
      <div class="topMenuButtonDiv" id="button14" onclick='setPage("13", true)'>
        <img id="button13_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
    </div>
    <div id="leftMenu">
      <div class="leftMenuButtonDiv" id="button1" onclick='setPage("1", true)'>
        <img id="button1_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
      <div class="leftMenuButtonDiv" id="button2" onclick='setPage("2", true);'>
        <img id="button2_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
      <div class="leftMenuButtonDiv" id="button3" onclick='setPage("3", true)'>
        <img id="button3_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
      <div class="leftMenuButtonDiv" id="button4" onclick='setPage("4", true)'>
        <img id="button4_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
      <div class="leftMenuButtonDiv" id="button5" onclick='setPage("5", true)'>
        <img id="button5_Img" alt="Arduino Components" style="width:100%;height:100%;" />
      </div>
    </div>
    <div id="pageArea">
      <div id="page1" style="display:block;width:100%;height:100%;">
        <div style="display:block;width:100%;height:5%;">
          <div class="slideIcon" style="float:left;padding-left:10px;padding-right:2px;">
            <img id="homeSlideIcon1" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="homeSlideIcon2" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="homeSlideIcon3" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="homeSlideIcon4" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="homeSlideIcon5" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="homeSlideIcon6" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="homeSlideIcon7" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="homeSlideIcon8" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="homeSlideIcon9" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;">
            <img id="homeSlideIcon10" style="width:100%;height:100%;" />
          </div>
          <div id="clock" style="width:auto;height:auto;float:right;padding-top:0px;margin-top:0px;">
            <b>
              <p class="ptext" id="clockText" style="display:inline;padding-right:4px;padding-top:0px;margin-top:0px;">00:00 am</p>
            </b>
          </div>
        </div>
        <div id="homepageblock1" style="display:block;width:100%;float:left;">
          <div style="display:block;width:50%;height:100%;float:left;">
            <div id="homeSensorDiv" style="display:block;width:100%;height:50%;">
              <center>
                <b><p class="p3text" id="homePageSensorTitle" style="margin-top:0.5vh;">EC Sensor</p> </b>              
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;">                  
              <center>
                <b><p class="p5text" id="homePageSensorValue" style="display:inline;">-</p>
                <p class="p2text" id="homePageIcon" style="display:inline;"></p></b>
              </center>
            </div>
          </div>
          <div style="display:block;width:50%;height:100%;float:left;">
            <div style="display:block;width:100%;height:30%;">
              <b>
                <p class="p3text" id="homePageTargetTitle" style="margin-top:0.5vh;text-align:center;">Target +/-</p>
              </b>
            </div>
            <div style="display:block;width:100%;height:20%;">
              <div style="display:block;width:50%;float:left;">
                <p class="p2text" style="margin:0px;text-align:center;">min</p>
              </div>
              <div style="display:block;width:50%;float:left;">
                <p class="p2text" style="margin:0px;text-align:center;">max</p>
              </div>
            </div>
            <div style="display:block;width:100%;height:50%;">
              <div style="display:block;width:50%;height:100%;float:left;">
                <b>
                  <p class="p4text" id="homePageMinTargetValue" style="text-align:center;margin-top:1.25vw;">-</p>
                </b>
              </div>
              <div style="display:block;width:50%;height:100%;float:left;">
                <b>
                  <p class="p4text" id="homePageMaxTargetValue" style="text-align:center;margin-top:1.25vw;">-</p>
                </b>
              </div>
            </div>
          </div>
        </div>
        <div id="homePageGraph" style="display:block;width:100%;height:45%;float:left;">
          <canvas id="graphCanvas" width="0" height="0">
            Your browser does not support the canvas element.
          </canvas>
        </div>
        <div id="leftSlideArrow" onclick="homePageSlider(-1)">
          <img class="leftSlideArrowIcon" alt="left scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
        <div id="rightSlideArrow" onclick="homePageSlider(1)">
          <img class="rightSlideArrowIcon" alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
      </div>

      <div id="page2" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;">
          <div class="slideIcon" style="float:left;padding-left:10px;padding-right:2px;">
            <img id="graphSlideIcon1" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="graphSlideIcon2" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="graphSlideIcon3" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="graphSlideIcon4" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="graphSlideIcon5" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="graphSlideIcon6" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="graphSlideIcon7" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="graphSlideIcon8" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="graphSlideIcon9" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;">
            <img id="graphSlideIcon10" style="width:100%;height:100%;" />
          </div>
        </div>
        <div style="display:block;width:100%;height:10%;float:left;">
          <b>
            <p class="p3text" id="graphPageSensorTitle" style="margin:0px;text-align:center;">EC Sensor</p>
          </b>
        </div>
        <div id="graphPageGraph" style="display:block;width:100%;height:85%;float:left;">
          <canvas id="graphCanvas2" width="0" height="0">
            Your browser does not support the canvas element.
          </canvas>
        </div>
        <div id="leftSlideArrow" onclick="graphSlider(-1)">
          <img class="leftSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
        <div id="rightSlideArrow" onclick="graphSlider(1)">
          <img class="rightSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
      </div>

      <div id="page3" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;">
          <div class="slideIcon" style="float:left;padding-left:10px;padding-right:2px;">
            <img id="maxMinSlideIcon1" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="maxMinSlideIcon2" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="maxMinSlideIcon3" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="maxMinSlideIcon4" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="maxMinSlideIcon5" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="maxMinSlideIcon6" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="maxMinSlideIcon7" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="maxMinSlideIcon8" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="maxMinSlideIcon9" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;">
            <img id="maxMinSlideIcon10" style="width:100%;height:100%;" />
          </div>
        </div>
        <div style="display:block;width:100%;height:20%;float:left;">
          <b>
            <p class="p3text" id="maxMinPageTitle" style="margin:0px;text-align:center;">EC History</p>
          </b>
        </div>
        <div style="display:block;width:100%;height:20%;float:left;">
          <div style="display:block;width:50%;height:100%;float:left;">
            <b>
              <p class="p3text" style="margin:0px;text-align:center;">Min</p>
            </b>
          </div>
          <div style="display:block;width:50%;height:100%;float:left;">
            <b>
              <p class="p3text" style="margin:0px;text-align:center;">Max</p>
            </b>
          </div>
        </div>
        <div id="maxMins" style="display:block;width:100%;height:50%;float:left;">
          <div style="display:block;width:50%;height:100%;float:left;">
            <div id="minHistoryValue" style="display:block;width:100%;display:flex;justify-content:center;align-items:initial;">
              <b>
                <p id="minValue" class="p5text" style="margin:0px;display:inline;">-</p>
              </b>
              <b>
                <p id="maxMin_min" class="p3text" style="margin:0px;display:inline;"></p>
              </b>
            </div>
          </div>
          <div style="display:block;width:50%;height:100%;float:left;">
            <div id="maxHistoryValue" style="display:block;width:100%;display:flex;justify-content:center;align-items:initial;">
              <b>
                <p id="maxValue" class="p5text" style="margin:0px;display:inline;">-</p>
              </b>
              <b>
                <p id="maxMin_max" class="p3text" style="margin:0px;display:inline;"></p>
              </b>
            </div>
          </div>
        </div>
        <div id="leftSlideArrow" onclick="maxMinPageSlider(-1)">
          <img class="leftSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
        <div id="rightSlideArrow" onclick="maxMinPageSlider(1)">
          <img class="rightSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
      </div>

      <div id="page4" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;"></div>
        <div style="display:block;width:100%;height:15%;float:left;">
          <b>
            <p class="p3text" style="margin:0px;text-align:center;">Save and load profiles</p>
          </b>
        </div>
        <div style="display:block;width:100%;height:14%;float:left;">
          <input
            type="text"
            class="p3text"
            value="Profile_1"
            id="profileInputBox_1"
            name="profileInputBox_1"
            style="width:40%;height:70%;float:left;margin-left:20px;overflow:hidden;text-align:center;box-sizing:border-box;-moz-box-sizing:border-box;-webkit-box-sizing:border-box"
            onchange="saveProfileName(0)"
            maxlength="10"/>
          <button 
            type="button"
            class="p2text"
            style="width:15%;height:70%;float:right;margin-right:10px;font-weight:bold;border-radius:8px;border-color:grey;background:grey;cursor:pointer;outline:none;margin-top:2px;display:flex;align-items:center;justify-content:center; "
            onclick="loadProfile(0)">
            Load
          </button>
          <button
            type="button"
            class="p2text"
            style="width:15%;height:70%;float:right;margin-right:20px;font-weight:bold;border-radius:8px;border-color:grey;background:grey;cursor:pointer;outline:none;margin-top:2px;display:flex;align-items:center;justify-content:center;"
            onclick="saveProfile(0)">
            Save
          </button>
        </div>
        <div style="display:block;width:100%;height:14%;float:left;">
          <input
            type="text"
            class="p3text"
            value="Profile_2"
            id="profileInputBox_2"
            name="profileInputBox_2"
            style="width:40%;height:70%;float:left;margin-left:20px;overflow:hidden;text-align:center;box-sizing:border-box;-moz-box-sizing:border-box;-webkit-box-sizing:border-box"
            onchange="saveProfileName(1)"
            maxlength="10"/>
          <button
            type="button"
            class="p2text"
            style="width:15%;height:70%;float:right;margin-right:10px;font-weight:bold;border-radius:8px;border-color:grey;background:grey;cursor:pointer;outline:none;margin-top:2px;display:flex;align-items:center;justify-content:center;"
            onclick="loadProfile(1)">
            Load
          </button>
          <button
            type="button"
            class="p2text"
            style="width:15%;height:70%;float:right;margin-right:20px;font-weight:bold;border-radius:8px;border-color:grey;background:grey;cursor:pointer;outline:none;margin-top:2px;display:flex;align-items:center;justify-content:center;"
            onclick="saveProfile(1)">
            Save
          </button>
        </div>
        <div style="display:block;width:100%;height:14%;float:left;">
          <input
            type="text"
            class="p3text"
            value="Profile_3"
            id="profileInputBox_3"
            name="profileInputBox_3"
            style="width:40%;height:70%;float:left;margin-left:20px;overflow:hidden;text-align:center;box-sizing:border-box;-moz-box-sizing:border-box;-webkit-box-sizing:border-box"
            onchange="saveProfileName(2)"
            maxlength="10"/>
          <button
            type="button"
            class="p2text"
            style="width:15%;height:70%;float:right;margin-right:10px;font-weight:bold;border-radius:8px;border-color:grey;background:grey;cursor:pointer;outline:none;margin-top:2px;display:flex;align-items:center;justify-content:center;"
            onclick="loadProfile(2)">
            Load
          </button>
          <button
            type="button"
            class="p2text"
            style="width:15%;height:70%;float:right;margin-right:20px;font-weight:bold;border-radius:8px;border-color:grey;background:grey;cursor:pointer;outline:none;margin-top:2px;display:flex;align-items:center;justify-content:center;"
            onclick="saveProfile(2)">
            Save
          </button>
        </div>
        <div style="display:block;width:100%;height:14%;float:left;">
          <input
            type="text"
            class="p3text"
            value="Profile_4"
            id="profileInputBox_4"
            name="profileInputBox_4"
            style="width:40%;height:70%;float:left;margin-left:20px;overflow:hidden;text-align:center;box-sizing:border-box;-moz-box-sizing:border-box;-webkit-box-sizing:border-box"
            onchange="saveProfileName(3)"
            maxlength="10"/>
          <button
            type="button"
            class="p2text"
            style="width:15%;height:70%;float:right;margin-right:10px;font-weight:bold;border-radius:8px;border-color:grey;background:grey;cursor:pointer;outline:none;margin-top:2px;display:flex;align-items:center;justify-content:center;"
            onclick="loadProfile(3)">
            Load
          </button>
          <button
            type="button"
            class="p2text"
            style="width:15%;height:70%;float:right;margin-right:20px;font-weight:bold;border-radius:8px;border-color:grey;background:grey;cursor:pointer;outline:none;margin-top:2px;display:flex;align-items:center;justify-content:center;"
            onclick="saveProfile(3)">
            Save
          </button>
        </div>
        <div style="display:block;width:100%;height:14%;float:left;">
          <input
            type="text"
            class="p3text"
            value="Profile_5"
            id="profileInputBox_5"
            name="profileInputBox_5"
            style="width:40%;height:70%;float:left;margin-left:20px;overflow:hidden;text-align:center;box-sizing:border-box;-moz-box-sizing:border-box;-webkit-box-sizing:border-box"
            onchange="saveProfileName(4)"
            maxlength="10"/>
          <button
            type="button"
            class="p2text"
            style="width:15%;height:70%;float:right;margin-right:10px;font-weight:bold;border-radius:8px;border-color:grey;background:grey;cursor:pointer;outline:none;margin-top:2px;display:flex;align-items:center;justify-content:center; "
            onclick="loadProfile(4)">
            Load
          </button>
          <button
            type="button"
            class="p2text"
            style="width:15%;height:70%;float:right;margin-right:20px;font-weight:bold;border-radius:8px;border-color:grey;background:grey;cursor:pointer;outline:none;margin-top:2px;display:flex;align-items:center;justify-content:center; "
            onclick="saveProfile(4)">
            Save
          </button>
        </div>
      </div>

      <div id="page5" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;"></div>
        <div style="display:block;width:100%;height:15%;float:left;">
          <b>
            <p class="p3text" style="margin:0px;text-align:center;">Settings</p>
          </b>
        </div>
        <div style="display:block;width:100%;height:76%;float:left;overflow:auto;">
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:0%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;display:inline;margin-top:20px;" class="p2text">Number of dosers</p>
                <p id="numDosers" style="padding-left:4vw;display:inline;margin-top:20px;" class="p2text">-</p>
              </b>
            </div>
            <div style="display:block;width:30%;height:90%;float:left;">
              <img class="lubutt" alt="down arrow"  style="float:right;padding-top:0%;padding-left:10%;" onclick="adjustnumDosers(1)" />
              <img class="ldbutt" alt="up arrow" style="float:right;padding-top:0%;" onclick="adjustnumDosers(-1)" />                   
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:3%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;display:inline;" class="p2text">Dosing interval</p>
                <p id="dosingInt" style="padding-left:5.26vw;display:inline;" class="p2text">-</p>
                <p style="display:inline;" class="p2text">hours</p>
              </b>
            </div>
            <div style="display:block;width:30%;height:90%;float:left;">
              <img class="lubutt" alt="down arrow" style="float:right;padding-top:0%;padding-left:10%;" onclick="adjustdosingInt(1)" />
              <img class="ldbutt" alt="up arrow" style="float:right;padding-top:0%;" onclick="adjustdosingInt(-1)" />                   
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:0%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;" class="p2text">Temperature in F</p>
              </b>
            </div>
            <div style="display:block;width:40%;height:100%;float:left;">
              <div class="settingbutton" style="display:block;float:right;">
                <div id="tempInFLeft" class="leftToggle"style="margin-top:0.2vw;"onclick="adjustTempInF(false)"></div>
                <div id="tempInFRight" class="rightToggle"style="margin-top:0.2vw;"onclick="adjustTempInF(true)"></div>
              </div>
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:3%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;" class="p2text">Height in inchs</p>
              </b>
            </div>
            <div style="display:block;width:40%;height:100%;float:left;">
              <div class="settingbutton" style="display:block;float:right;">
                <div
                  id="heightInInchsLeft"class="leftToggle"style="margin-top:0.2vw;"onclick="adjustHeightInInchs(false)"></div>
                <div
                  id="heightInInchsRight" class="rightToggle"style="margin-top:0.2vw;"onclick="adjustHeightInInchs(true)"></div>
              </div>
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:3%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;" class="p2text">EC to TDS</p>
              </b>
            </div>
            <div style="display:block;width:40%;height:100%;float:left;">
              <div class="settingbutton" style="display:block;float:right;">
                <div id="ecToTdsLeft"class="leftToggle"style="margin-top:0.2vw;"onclick="adjustEcToTds(false)"></div>
                <div id="ecToTdsRight" class="rightToggle"style="margin-top:0.2vw;" onclick="adjustEcToTds(true)"></div>
              </div>
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:3%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;" class="p2text">Disable Co2 warnings</p>
              </b>
            </div>
            <div style="display:block;width:40%;height:100%;float:left;">
              <div class="settingbutton" style="display:block;float:right;">
                <div
                  id="disableCo2WarnLeft"
                  class="leftToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableCo2Warn(false)">
                </div>
                <div
                  id="disableCo2WarnRight"
                  class="rightToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableCo2Warn(true)">
                </div>
              </div>
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:3%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;" class="p2text">Disable EC warnings</p>
              </b>
            </div>
            <div style="display:block;width:40%;height:100%;float:left;">
              <div class="settingbutton" style="display:block;float:right;">
                <div
                  id="disableEcWarnLeft"
                  class="leftToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableEcWarn(false)">
                </div>
                <div
                  id="disableEcWarnRight"
                  class="rightToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableEcWarn(true)">
                </div>
              </div>
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:3%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;" class="p2text">Disable PPM warnings</p>
              </b>
            </div>
            <div style="display:block;width:40%;height:100%;float:left;">
              <div class="settingbutton" style="display:block;float:right;">
                <div
                  id="disablePpmWarnLeft"
                  class="leftToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisablePpmWarn(false)">
                </div>
                <div
                  id="disablePpmWarnRight"
                  class="rightToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisablePpmWarn(true)">
                </div>
              </div>
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:3%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;" class="p2text">Disable PH warnings</p>
              </b>
            </div>
            <div style="display:block;width:40%;height:100%;float:left;">
              <div class="settingbutton" style="display:block;float:right;">
                <div
                  id="disablePhWarnLeft"
                  class="leftToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisablePhWarn(false)">
                </div>
                <div
                  id="disablePhWarnRight"
                  class="rightToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisablePhWarn(true)">
                </div>
              </div>
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:3%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;" class="p2text">Disable water temp warnings</p>
              </b>
            </div>
            <div style="display:block;width:40%;height:100%;float:left;">
              <div class="settingbutton" style="display:block;float:right;">
                <div
                  id="disableWaterTempWarnLeft"
                  class="leftToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableWaterTempWarn(false)"></div>
                <div id="disableWaterTempWarnRight"
                  class="rightToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableWaterTempWarn(true)">
                </div>
              </div>
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:3%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;" class="p2text">Disable water height warnings</p>
              </b>
            </div>
            <div style="display:block;width:40%;height:100%;float:left;">
              <div class="settingbutton" style="display:block;float:right;">
                <div
                  id="disableWaterHeightWarnLeft"
                  class="leftToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableWaterHeightWarn(false)"></div>
                <div
                  id="disableWaterHeightWarnRight"
                  class="rightToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableWaterHeightWarn(true)">
                </div>
              </div>
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:3%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;" class="p2text">Disable air temp warnings</p>
              </b>
            </div>
            <div style="display:block;width:40%;height:100%;float:left;">
              <div class="settingbutton" style="display:block;float:right;">
                <div
                  id="disableAirTempWarnLeft"
                  class="leftToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableAirTempWarn(false)"></div>
                <div
                  id="disableAirTempWarnRight"
                  class="rightToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableAirTempWarn(true)">
                </div>
              </div>
            </div>
          </div>
          <div style="display:block;width:95%;height:15%;float:left;padding-left:5%;padding-top:3%;">
            <div style="display:block;width:60%;height:100%;float:left;">
              <b>
                <p style="text-align:left;" class="p2text">Disable humidity warnings</p>
              </b>
            </div>
            <div style="display:block;width:40%;height:100%;float:left;">
              <div class="settingbutton" style="display:block;float:right;">
                <div
                  id="disableHumidityWarnLeft"
                  class="leftToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableHumidityWarn(false)"></div>
                <div
                  id="disableHumidityWarnRight"
                  class="rightToggle"style="margin-top:0.2vw;"
                  onclick="adjustdisableHumidityWarn(true)">
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <div id="page6" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;"></div>
        <div style="display:block;width:100%;height:20%;float:left;">
          <b>
            <p ID="TargetEcTitle" style="margin:0px;text-align:center;" class="p3text">Target EC</p>
          </b>
        </div>
        <div style="display:block;width:100%;height:20%;float:left;">
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <b>
              <p style="margin:0px;text-align:center;" class="p3text">Minimum</p>
            </b>
          </div>
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <b>
              <p style="margin:0px;text-align:center;" class="p3text">Maximum</p>
            </b>
          </div>
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <b>
              <p style="margin:0px;text-align:center;" class="p3text">Tolerance</p>
            </b>
          </div>
        </div>
        <div style="display:block;width:100%;height:50%;float:left;">
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <div style="display:block;width:100%;height:50%;float:left;">
              <b>
                <p id="targetMinEc" style="margin:0px;text-align:center;" class="p5text">-</p>
              </b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustMinEc(-0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustMinEc(0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <div style="display:block;width:100%;height:50%;float:left;">
              <b>
                <p id="targetMaxEc" style="margin:0px;text-align:center;" class="p5text">-</p>
              </b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustMaxEc(-0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustMaxEc(0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <div style="display:block;width:100%;height:50%;float:left;">
              <b>
                <p id="targetEcOffset" style="margin:0px;text-align:center;" class="p5text">-</p>
              </b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustEcOffset(-0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustEcOffset(0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
            </div>
          </div>
        </div>
      </div>

      <div id="page7" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;"></div>
        <div style="display:block;width:100%;height:20%;float:left;">
          <b>
            <p style="margin:0px;text-align:center;" class="p3text">Target PH</p>
          </b>
        </div>
        <div style="display:block;width:100%;height:20%;float:left;">
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <b>
              <p style="margin:0px;text-align:center;" class="p3text">Minimum</p>
            </b>
          </div>
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <b>
              <p style="margin:0px;text-align:center;" class="p3text">Maximum</p>
            </b>
          </div>
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <b>
              <p style="margin:0px;text-align:center;" class="p3text">Tolerance</p>
            </b>
          </div>
        </div>
        <div style="display:block;width:100%;height:50%;float:left;">
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <div style="display:block;width:100%;height:50%;float:left;">
              <b>
                <p id="targetMinPh" style="margin:0px;text-align:center;" class="p5text">-</p>
              </b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustMinPh(-0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustMinPh(0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <div style="display:block;width:100%;height:50%;float:left;">
              <b>
                <p id="targetMaxPh" style="margin:0px;text-align:center;" class="p5text">-</p>
              </b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustMaxPh(-0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustMaxPh(0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.33%;height:100%;float:left;">
            <div style="display:block;width:100%;height:50%;float:left;">
              <b>
                <p id="targetPhOffset" style="margin:0px;text-align:center;" class="p5text">-</p>
              </b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustPhOffset(-0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustPhOffset(0.01)" />
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
            </div>
          </div>
        </div>
      </div>

      <div id="page8" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;">
          <div class="slideIcon" style="float:left;padding-left:10px;padding-right:2px;">
            <img id="co2SlideIcon1"  style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="co2SlideIcon2" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="co2SlideIcon3" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="co2SlideIcon4" style="width:100%;height:100%;" />
          </div>
        </div>
        <div id="leftSlideArrow" onclick="co2PageSlider(-1)">
          <img class="leftSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
        <div id="rightSlideArrow" onclick="co2PageSlider(1)">
          <img class="rightSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
        <div id="co2page1" style="display:block;width:100%;height:94%;float:left;">
          <div style="display:block;width:100%;height:20%;float:left;">
            <b>
              <p style="margin:0px;text-align:center;" class="p3text">Target Co2</p>
            </b>
          </div>
          <div style="display:block;width:100%;height:20%;float:left;">
            <div style="display:block;width:50%;height:100%;float:left;">
              <b>
                <p style="margin:0px;text-align:center;" class="p3text">Target</p>
              </b>
            </div>
            <div style="display:block;width:50%;height:100%;float:left;">
              <b>
                <p style="margin:0px;text-align:center;" class="p3text">Tolerance</p>
              </b>
            </div>
          </div>
          <div style="display:block;width:50%;height:60%;float:left;">
            <div style="display:block;width:100%;height:50%;float:left;">
              <center>
                <b>
                  <p id="targetCo2" class="p5text" style="margin:0px;display:inline;">-</p>
                </b>
                <b>
                  <p id="targetCo2Symbol" class="p3text" style="margin:0px;display:inline;">ppm</p>
                </b>
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:15%;height:50%;float:left;"></div>
              <div style="display:block;width:25%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustTargetCo2(-1)" style="float:right;"/>
              </div>
              <div style="display:block;width:20%;height:50%;float:left;"></div>
              <div style="display:block;width:25%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustTargetCo2(1)" style="float:left;/>
              </div>
              <div style="display:block;width:15%;height:50%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:50%;height:60%;float:left;">
            <div style="display:block;width:100%;height:50%;float:left;">
              <center>
                <b>
                  <p id="targetCo2Offset" class="p5text" style="margin:0px;display:inline;">-</p>
                </b>
                <b>
                  <p class="p3text" style="margin:0px;display:inline;">ppm</p>
                </b>
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:15%;height:50%;float:left;"></div>
              <div style="display:block;width:25%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustTargetCo2Offset(-1)" style="float:right;"/>
              </div>
              <div style="display:block;width:20%;height:50%;float:left;"></div>
              <div style="display:block;width:25%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustTargetCo2Offset(1)" style="float:left;/>
              </div>
              <div style="display:block;width:15%;height:50%;float:left;"></div>
            </div>
          </div>
        </div>
        <div id="co2page2" style="display:none;width:94%;height:100%;float:left;">
          <div style="display:block;width:100%;height:20%;float:left;">
            <b>
              <p style="margin:0px;text-align:center;" class="p3text">Dimensions</p>
            </b>
          </div>
          <div style="display:block;width:100%;height:20%;float:left;">
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b>
                <p style="margin:0px;text-align:center;" class="p3text">Length</p>
              </b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b>
                <p style="margin:0px;text-align:center;" class="p3text">Width</p>
              </b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b>
                <p style="margin:0px;text-align:center;" class="p3text">Height</p>
              </b>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div id="minCo2Div" style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
              <center>
                <b>
                  <p id="co2RoomLength" class="p5text" style="margin:0px;display:inline;">-</p>
                </b>
                <b>
                  <p name="cm" id="targetCo2Symbol" class="p3text" style="margin:0px;display:inline;">cm</p>
                </b>
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustCo2RoomLength(-1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustCo2RoomLength(1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div id="maxCo2Div" style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
              <center>
                <b>
                  <p id="co2RoomWidth" class="p5text" style="margin:0px;display:inline;">-</p>
                </b>
                <b>
                  <p name="cm" class="p3text" style="margin:0px;display:inline;">cm</p>
                </b>
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustCo2RoomWidth(-1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustCo2RoomWidth(1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div id="co2OffsetDiv" style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
              <center>
                <b>
                  <p id="co2RoomHeight" class="p5text" style="margin:0px;display:inline;">-</p>
                </b>
                <b>
                  <p name="cm" class="p3text" style="margin:0px;display:inline;">cm</p>
                </b>
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustCo2RoomHeight(-1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustCo2RoomHeight(1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
        </div>
        <div id="co2page3" style="display:none;width:94%;height:100%;float:left;">
          <div style="display:block;width:100%;height:20%;float:left;">
            <b>
              <p style="margin:0px;text-align:center;" class="p3text">Co2 Settings</p>
            </b>
          </div>
          <div style="display:block;width:100%;height:20%;float:left;">
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b>
                <p style="margin:0px;text-align:center;" class="p3text">Flow rate</p>
              </b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b>
                <p style="margin:0px;text-align:center;" class="p3text">Time</p>
              </b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b>
                <p style="margin:0px;text-align:center;" class="p3text">Fans off</p>
              </b>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
              <center>
                <b>
                  <p id="co2FlowRate" class="p4Ltext" style="margin:0px;display:inline;">-</p>
                </b>
                <b>
                  <p id="co2FlowRateSymbol" class="p3text" style="margin:0px;display:inline;">ltrs</p>
                </b>
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustCo2FlowRate(-0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustCo2FlowRate(0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
              <center>
                <b>
                  <p id="co2Interval" class="p4Ltext" style="margin:0px;display:inline;">-</p>
                </b>
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustCo2Interval(-1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustCo2Interval(1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
              <center>
                <b>
                  <p id="co2DisableFanTimer" class="p4Ltext" style="margin:0px;display:inline;">-</p>
                </b>
                <b>
                  <p class="p3text" style="margin:0px;display:inline;">mins</p>
                </b>
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustCo2FanDisableTime(-1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustCo2FanDisableTime(1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
        </div>
        <div id="co2page4" style="display:none;width:94%;height:100%;float:left;">
          <div style="display:block;width:100%;height:20%;float:left;">
            <b>
              <p style="margin:0px;text-align:center;" class="p3text">Co2 Settings</p>
            </b>
          </div>
          <div style="display:block;width:100%;height:20%;float:left;">
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Manual duration</p> </b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Manual gas duration</p> </b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Disable Co2</p> </b>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div style="display:block;width:20%;height:100%;float:left;"></div>
            <div style="display:block;width:60%;height:30%;float:left;margin-top:15%;margin-left:5%;">
              <div style="display:block;width:100%;height:50%;margin: 0 auto;">
                
                <div
                  id="gasTimeEnabledRight"
                  class="leftToggle"
                  onclick="gasTimeEnabled(false)">
                </div>
                <div
                  id="gasTimeEnabledLeft"
                  class="rightToggle"
                  onclick="gasTimeEnabled(true)">
                </div>
          
              </div>
            </div>
            <div style="display:block;width:20%;height:100%;float:left;"></div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div id="maxCo2Div" style="display:block;width:100%;height:50%;float:left;align-items:baseline;">
              <center>
                <b><p id="manualGasDurationTimer" class="p5text" style="margin:0px;display:inline;">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">mins</p></b>
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:50%;float:left;"></div>
              <div style="display:block;width:20%;height:50%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustManualGasDurationTime(-1)" />
              </div>
              <div style="display:block;width:20%;height:50%;float:left;"></div>
              <div style="display:block;width:20%;height:50%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustManualGasDurationTime(1)" />
              </div>
              <div style="display:block;width:20%;height:50%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div style="display:block;width:20%;height:100%;float:left;"></div>
            <div style="display:block;width:60%;height:30%;float:left;margin-top:15%;">
              <div style="display:block;width:100%;height:50%;float:left;margin-left:5%;">
                <div
                  id="disableCo2ControlRight"
                  class="leftToggle"
                  onclick="disableCo2Control(false)">
                </div>
                <div
                  id="disableCo2ControlLeft"
                  class="rightToggle"
                  onclick="disableCo2Control(true)">
                </div>
              </div>
            </div>
            <div style="display:block;width:20%;height:100%;float:left;"></div>
          </div>
        </div>
      </div>

      <div id="page9" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;">
          <div class="slideIcon" style="float:left;padding-left:10px;padding-right:2px;">
            <img id="waterSlideIcon1"  style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="waterSlideIcon2" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="waterSlideIcon3" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="waterSlideIcon4" style="width:100%;height:100%;" />
          </div>
        </div>
        <div id="leftSlideArrow" onclick="waterPageSlider(-1)">
          <img class="leftSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
        <div id="rightSlideArrow" onclick="waterPageSlider(1)">
          <img class="rightSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
        <div id="waterPage1" style="display:block;width:94%;height:100%;float:left;">
          <div style="display:block;width:100%;height:20%;float:left;">
            <b><p style="margin:0px;text-align:center;" class="p3text">Target water level</p></b>
          </div>
          <div style="display:block;width:100%;height:20%;float:left;">
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Minimum</p></b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Maximum</p></b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Tolerance</p></b>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div id="targetMinWaterLvlDiv" style="display:block;width:100%;height:45%;float:left;display:flex;justify-content:center;align-items:initial;">
              <b><p id="targetMinWaterLvl" class="p5text" style="margin:0px;display:inline;">-</p></b>
              <b><p name="waterIcon" class="p3text" style="margin:0px;display:inline;">cm</p></b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustMinWaterLvl(-0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustMinWaterLvl(0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div id="targetMaxWaterLvlDiv" style="display:block;width:100%;height:45%;float:left;display:flex;justify-content:center;align-items:initial;">
              <b><p id="targetMaxWaterLvl" class="p5text" style="margin:0px;display:inline;">-</p></b>
              <b><p name="waterIcon" class="p3text" style="margin:0px;display:inline;">cm</p></b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustMaxWaterLvl(-0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustMaxWaterLvl(0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div id="targetWaterLvlOffsetDiv" style="display:block;width:100%;height:45%;float:left;display:flex;justify-content:center;align-items:initial;">
              <b><p id="targetWaterLvlOffset" class="p5text" style="margin:0px;display:inline;">-</p></b>
              <b><p name="waterIcon" class="p3text" style="margin:0px;display:inline;">cm</p></b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustWaterLvlOffset(-0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustWaterLvlOffset(0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
        </div>
        <div id="waterPage2" style="display:none;width:94%;height:100%;float:left;">
          <div style="display:block;width:100%;height:20%;float:left;">
            <b><p style="margin:0px;text-align:center;" class="p3text">Target water temp</p></b>
          </div>
          <div style="display:block;width:100%;height:20%;float:left;">
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Minimum</p></b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Maximum</p></b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Tolerance</p> </b>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div id="targetMinWaterTempDiv" style="display:block;width:100%;height:45%;float:left;display:flex;justify-content:center;align-items:initial;">
              <b><p id="targetMinWaterTemp" class="p5text" style="margin:0px;display:inline;">-</p></b>
              <b><p name="waterIcon" class="p3text" style="margin:0px;display:inline;">c</p></b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustMinWaterTemp(-0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustMinWaterTemp(0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div id="targetMaxWaterTempDiv" style="display:block;width:100%;height:45%;float:left;display:flex;justify-content:center;align-items:initial;">
              <b><p id="targetMaxWaterTemp" class="p5text" style="margin:0px;display:inline;">-</p></b>
              <b><p name="waterIcon" class="p3text" style="margin:0px;display:inline;">c</p></b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustMaxWaterTemp(-0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustMaxWaterTemp(0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div id="targetWaterTempOffsetDiv" style="display:block;width:100%;height:45%;float:left;display:flex;justify-content:center;align-items:initial;">
              <b><p id="targetWaterTempOffset" class="p5text" style="margin:0px;display:inline;">-</p> </b><b><p name="waterIcon" class="p3text" style="margin:0px;display:inline;">c</p></b>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustWaterTempOffset(-0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
              <div style="display:block;width:20%;height:55%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustWaterTempOffset(0.1)" />
              </div>
              <div style="display:block;width:20%;height:55%;float:left;"></div>
            </div>
          </div>
        </div>
        <div id="waterPage3" style="display:none;width:94%;height:100%;float:left;">
          <div style="display:block;width:60%;height:100%;float:left;">
            <div style="display:block;width:100%;height:20%;float:left;">
              <b><p style="margin:0px;text-align:center;padding-left:10%;" class="p3text">Refill tank settings</p></b>
            </div>
            <div style="width:90%;height:60%;float:left;padding-left:10%;">
              <div class="calanderDayLeft" id="calanderDay1" onclick="updateCalanderDays(1)"><p>1</p></div>
              <div class="calanderDayLeft" id="calanderDay2" onclick="updateCalanderDays(2)"><p>2</p></div>
              <div class="calanderDayLeft" id="calanderDay3" onclick="updateCalanderDays(3)"><p>3</p></div>
              <div class="calanderDayLeft" id="calanderDay4" onclick="updateCalanderDays(4)"><p>4</p></div>
              <div class="calanderDayLeft" id="calanderDay5" onclick="updateCalanderDays(5)"><p>5</p></div>
              <div class="calanderDayLeft" id="calanderDay6" onclick="updateCalanderDays(6)"><p>6</p></div>
              <div class="calanderDayRight" id="calanderDay7" onclick="updateCalanderDays(7)"><p>7</p></div>
              <div class="calanderDayLeft" id="calanderDay8" onclick="updateCalanderDays(8)"><p>8</p></div>
              <div class="calanderDayLeft" id="calanderDay9" onclick="updateCalanderDays(9)"><p>9</p></div>
              <div class="calanderDayLeft" id="calanderDay10" onclick="updateCalanderDays(10)"><p>10</p></div>
              <div class="calanderDayLeft" id="calanderDay11" onclick="updateCalanderDays(11)"><p>11</p></div>
              <div class="calanderDayLeft" id="calanderDay12" onclick="updateCalanderDays(12)"><p>12</p></div>
              <div class="calanderDayLeft" id="calanderDay13" onclick="updateCalanderDays(13)"><p>13</p></div>
              <div class="calanderDayRight" id="calanderDay14" onclick="updateCalanderDays(14)"><p>14</p></div>
              <div class="calanderDayLeft" id="calanderDay15" onclick="updateCalanderDays(15)"><p>15</p></div>
              <div class="calanderDayLeft" id="calanderDay16" onclick="updateCalanderDays(16)"><p>16</p></div>
              <div class="calanderDayLeft" id="calanderDay17" onclick="updateCalanderDays(17)"><p>17</p></div>
              <div class="calanderDayLeft" id="calanderDay18" onclick="updateCalanderDays(18)"><p>18</p></div>
              <div class="calanderDayLeft" id="calanderDay19" onclick="updateCalanderDays(19)"><p>19</p></div>
              <div class="calanderDayLeft" id="calanderDay20" onclick="updateCalanderDays(20)"><p>20</p></div>
              <div class="calanderDayRight" id="calanderDay21" onclick="updateCalanderDays(21)"><p>21</p></div>
              <div class="calanderDayLeft" id="calanderDay22" onclick="updateCalanderDays(22)"><p>22</p></div>
              <div class="calanderDayLeft" id="calanderDay23" onclick="updateCalanderDays(23)"><p>23</p></div>
              <div class="calanderDayLeft" id="calanderDay24" onclick="updateCalanderDays(24)"><p>24</p></div>
              <div class="calanderDayBottom" id="calanderDay25" onclick="updateCalanderDays(25)"><p>25</p></div>
              <div class="calanderDayBottom" id="calanderDay26" onclick="updateCalanderDays(26)"><p>26</p></div>
              <div class="calanderDayBottom" id="calanderDay27" onclick="updateCalanderDays(27)"><p>27</p></div>
              <div class="calanderDayEnd" id="calanderDay28" onclick="updateCalanderDays(28)"><p>28</p></div>
              <div class="calanderDayBottom" id="calanderDay29" onclick="updateCalanderDays(29)"><p>29</p></div>
              <div class="calanderDayBottom" id="calanderDay30" onclick="updateCalanderDays(30)"><p>30</p></div>
              <div class="calanderDayEnd" id="calanderDay31" onclick="updateCalanderDays(31)"><p>31</p></div>
            </div>
            <div style="display:block;width:100%;height:20%;float:left;">
              <div style="display:block;width:70%;height:100%;float:left;">
                <b><p class="p3text" id="refillTankTime" style="text-align:left;padding:0px;margin:0px;padding-left:10%;">Time -</p></b>
              </div>
              <div style="display:block;width:30%;height:100%;float:left;">
                <div style="display:block;width:20%;height:45%;float:left;">
                  <img class="ldbutt" alt="up arrow" onclick="adjustRefillTankTime(-1)" />
                </div>
                <div style="display:block;width:20%;height:100%;float:left;"></div>
                <div style="display:block;width:20%;height:45%;float:left;">
                  <img class="lubutt" alt="down arrow" onclick="adjustRefillTankTime(1)" />
                </div>
              </div>
            </div>
          </div>
          <div style="display:block;width:40%;height:100%;float:left;">
            <div style="display:block;width:100%;height:20%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Disable drain and refill</p></b>
            </div>
            <div style="display:block;width:25%;height:100%;float:left;"></div>
            <div style="display:block;width:50%;height:20%;float:left;margin-top:25%;">
              <div style="display:block;width:100%;height:50%;float:left;">
                <div
                  id="dRefillDrainLeft"
                  class="leftToggle"
                  onclick="adjustDisableRefillAndDrain(false)">
                </div>
                <div
                  id="dRefillDrainRight"
                  class="rightToggle"
                  onclick="adjustDisableRefillAndDrain(true)">
                </div>
              </div>
            </div>
            <div style="display:block;width:25%;height:100%;float:left;"></div>
          </div>
        </div>
        <div id="waterPage4" style="display:none;width:94%;height:100%;float:left;">
          <div style="display:block;width:100%;height:20%;float:left;">
            <b><p id="doserPageSensorTitle" style="margin:0px;text-align:center;" class="p3text">Refill dosing Mls</p></b>
          </div>
          <div id="doserPage" style="display:block;width:100%;height:70%;float:left;">
            <div style="display:block;width:8%;height:100%;float:left;"></div>
            <div style="display:block;width:20%;height:100%;float:left;position:relative;">
              <div style="display:block;width:100%;height:40%;float:left;">
                <img class="doserL" style="width:100%;height:100%; filter:grayscale(100%);" />
              </div>
              <div style="display:block;width:100%;height:15%;float:left;">
                <input class="p3text" type="button" value="OFF" id="waterEcButton1" style="background:Transparent;border:none;border-radius:12px;width:40%;height:70%;font-weight:bold;margin:5% 30%;outline:none;" />
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <center>
                  <b><p id="d1mls" style="margin:0px;display:inline;" class="p4text">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">mls</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">            
                <img class="ldbutt" alt="up arrow"  onclick="adjustWaterDoserMls(-1, 'd1mls')" />
                <img class="lubutt" alt="down arrow" onclick="adjustWaterDoserMls(1, 'd1mls')" />               
              </div>
              <b><p class="p3text" id="waterDoser1" style="margin:0px;text-align:center;position:absolute;top:5px;left:5px;">1</p></b>
            </div>
            <div style="display:block;width:4%;height:100%;float:left;"></div>
            <div style="display:block;width:20%;height:100%;float:left;position:relative;">
              <div style="display:block;width:100%;height:40%;float:left;">
                <img class="doserL" style="width:100%;height:100%; filter:grayscale(100%);" />
              </div>
              <div style="display:block;width:100%;height:15%;float:left;">
                <input class="p3text" type="button" value="OFF" id="waterEcButton2" style="background:Transparent;border:none;border-radius:12px;width:40%;height:70%;font-weight:bold;margin:5% 30%;outline:none;" />
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <center>
                  <b><p id="d2mls" style="margin:0px;display:inline;" class="p4text">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">mls</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustWaterDoserMls(-1, 'd2mls')" />  
                <img class="lubutt" alt="down arrow" onclick="adjustWaterDoserMls(1, 'd2mls')" />                          
              </div>
              <b><p class="p3text" id="waterDoser2" style="margin:0px;text-align:center;position:absolute;top:5px;left:5px;">2</p></b>
            </div>
            <div style="display:block;width:4%;height:100%;float:left;"></div>
            <div style="display:block;width:20%;height:100%;float:left;position:relative;">
              <div style="display:block;width:100%;height:40%;float:left;">
                <img class="doserL" style="width:100%;height:100%; filter:grayscale(100%);" />
              </div>
              <div style="display:block;width:100%;height:15%;float:left;">
                <input class="p3text" type="button" value="OFF" id="waterEcButton3" style="background:Transparent;border:none;border-radius:12px;width:40%;height:70%;font-weight:bold;margin:5% 30%;outline:none;" />
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <center>
                  <b><p id="d3mls" style="margin:0px;display:inline;" class="p4text">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">mls</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustWaterDoserMls(-1, 'd3mls')" />
                <img class="lubutt" alt="down arrow" onclick="adjustWaterDoserMls(1, 'd3mls')" />
              </div>
              <b><p class="p3text" id="waterDoser3" style="margin:0px;text-align:center;position:absolute;top:5px;left:5px;">3</p></b>
            </div>
            <div style="display:block;width:4%;height:100%;float:left;"></div>
            <div style="display:block;width:20%;height:100%;float:left;position:relative;">
              <div style="display:block;width:100%;height:40%;float:left;">
                <img class="doserL" style="width:100%;height:100%; filter:grayscale(100%);"/>
              </div>

              <div style="display:block;width:100%;height:15%;float:left;">
                <input class="p3text" type="button" value="OFF" id="waterEcButton4" style="background:Transparent;border:none;border-radius:12px;width:40%;height:70%;font-weight:bold;margin:5% 30%;outline:none;" />
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <center>
                  <b><p id="d4mls" style="margin:0px;display:inline;" class="p4text">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">mls</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustWaterDoserMls(-1, 'd4mls')" />
                <img class="lubutt" alt="down arrow" onclick="adjustWaterDoserMls(1, 'd4mls')" />
              </div>
              <b><p class="p3text" id="waterDoser4" style="margin:0px;text-align:center;position:absolute;top:5px;left:5px;">4</p></b>
            </div>
          </div>
        </div>
      </div>
      <div id="page10" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;">
          <div class="slideIcon" style="float:left;padding-left:10px;padding-right:2px;">
            <img id="doserSlideIcon1"  style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="doserSlideIcon2" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="doserSlideIcon3" style="width:100%;height:100%;" />
          </div>
        </div>
        <div id="leftSlideArrow" onclick="doserPageSlider(-1)"><img class="leftSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" /></div>
        <div id="rightSlideArrow" onclick="doserPageSlider(1)"><img class="rightSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" /></div>
        <div style="width:94%;height:100%;float:left;">
          <div style="display:block;width:100%;height:20%;float:left;">
            <b><p id="doserPageSensorTitle" style="margin:0px;text-align:center;" class="p3text">Dosing Mls</p></b>
          </div>
          <div id="doserPage" style="display:block;width:100%;height:70%;float:left;">
            <div style="display:block;width:8%;height:100%;float:left;"></div>
            <div style="display:block;width:20%;height:100%;float:left;position:relative;">
              <div style="display:block;width:100%;height:40%;float:left;">
                <img class="doserL" style="width:100%;height:100%; filter:grayscale(100%);" /></div>
              <div style="display:block;width:100%;height:15%;float:left;">
                <input
                  class="p3text"
                  onclick="changeDoserButton('ecButton1')"
                  type="button"
                  value="OFF"
                  id="ecButton1"
                  style="background:Transparent;border:none;border-radius:12px;width:40%;height:70%;font-weight:bold;margin:5% 30%;cursor:pointer;outline:none;"
                />
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <center>
                  <b><p id="doserMls_1" style="margin:0px;display:inline;" class="p4text">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">mls</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustDoserMls(-1, 'doserMls_1')" />
                <img class="lubutt" alt="down arrow" onclick="adjustDoserMls(1, 'doserMls_1')" />
              </div>
              <b><p class="p3text" id="doser1" style="margin:0px;text-align:center;position:absolute;top:5px;left:5px;">1</p></b>
            </div>
            <div style="display:block;width:4%;height:100%;float:left;"></div>
            <div style="display:block;width:20%;height:100%;float:left;position:relative;">
              <div style="display:block;width:100%;height:40%;float:left;">
                <img class="doserL" style="width:100%;height:100%; filter:grayscale(100%);" />
              </div>
              <div style="display:block;width:100%;height:15%;float:left;">
                <input
                  class="p3text"
                  onclick="changeDoserButton('ecButton2')"
                  type="button"
                  value="OFF"
                  id="ecButton2"
                  style="background:Transparent;border:none;border-radius:12px;width:40%;height:70%;font-weight:bold;margin:5% 30%;cursor:pointer;outline:none;"
                />
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <center>
                  <b><p id="doserMls_2" style="margin:0px;display:inline;" class="p4text">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">mls</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustDoserMls(-1, 'doserMls_2')" />
                <img class="lubutt" alt="down arrow" onclick="adjustDoserMls(1, 'doserMls_2')" />
              </div>
              <b><p class="p3text" id="doser2" style="margin:0px;text-align:center;position:absolute;top:5px;left:5px;">2</p></b>
            </div>
            <div style="display:block;width:4%;height:100%;float:left;"></div>
            <div style="display:block;width:20%;height:100%;float:left;position:relative;">
              <div style="display:block;width:100%;height:40%;float:left;">
              <img class="doserL" style="width:100%;height:100%; filter:grayscale(100%);" /></div>
              <div style="display:block;width:100%;height:15%;float:left;">
                <input
                  class="p3text"
                  onclick="changeDoserButton('ecButton3')"
                  type="button"
                  value="OFF"
                  id="ecButton3"
                  style="background:Transparent;border:none;border-radius:12px;width:40%;height:70%;font-weight:bold;margin:5% 30%;cursor:pointer;outline:none;"
                />
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <center>
                  <b><p id="doserMls_3" style="margin:0px;display:inline;" class="p4text">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">mls</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustDoserMls(-1, 'doserMls_3')" />
                <img class="lubutt" alt="down arrow" onclick="adjustDoserMls(1, 'doserMls_3')" />
              </div>
              <b><p class="p3text" id="doser3" style="margin:0px;text-align:center;position:absolute;top:5px;left:5px;">3</p></b>
            </div>
            <div style="display:block;width:4%;height:100%;float:left;"></div>
            <div style="display:block;width:20%;height:100%;float:left;position:relative;">
              <div style="display:block;width:100%;height:40%;float:left;">
                <img class="doserL" style="width:100%;height:100%; filter:grayscale(100%);" />
              </div>
              <div style="display:block;width:100%;height:15%;float:left;">
                <input
                  class="p3text"
                  onclick="changeDoserButton('ecButton4')"
                  type="button"
                  value="OFF"
                  id="ecButton4"
                  style="background:Transparent;border:none;border-radius:12px;width:40%;height:70%;font-weight:bold;margin:5% 30%;cursor:pointer;outline:none;"
                />
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <center>
                  <b><p id="doserMls_4" style="margin:0px;display:inline;" class="p4text">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">mls</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:20%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustDoserMls(-1, 'doserMls_4')" />
                <img class="lubutt" alt="down arrow" onclick="adjustDoserMls(1, 'doserMls_4')" />
              </div>
              <b><p class="p3text" id="doser4" style="margin:0px;text-align:center;position:absolute;top:5px;left:5px;">4</p></b>
            </div>
          </div>
        </div>
      </div>
      <div id="page11" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;"></div>
        <div style="width:94%;height:100%;float:left;">
          <div style="display:block;width:100%;height:20%;float:left;">
            <b><p style="margin:0px;text-align:center;" class="p3text">Light settings</p></b>
          </div>
          <div style="display:block;width:100%;height:20%;float:left;">
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Light on</p></b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Light off</p></b>
            </div>
            <div style="display:block;width:33.3%;height:100%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Off/Auto/On</p></b>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div style="display:block;width:100%;height:50%;float:left;align-items:baseline;">
              <center>
                <b><p id="lightOnTime" class="p4Ltext" style="margin:0px;display:inline;">-</p></b>
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:50%;float:left;"></div>
              <div style="display:block;width:20%;height:50%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustLightOn(-1)" />
              </div>
              <div style="display:block;width:20%;height:50%;float:left;"></div>
              <div style="display:block;width:20%;height:50%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustLightOn(1)" />
              </div>
              <div style="display:block;width:20%;height:50%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div style="display:block;width:100%;height:50%;float:left;align-items:baseline;">
              <center>
                <b><p id="lightOffTime" class="p4Ltext" style="margin:0px;display:inline;">-</p></b>
              </center>
            </div>
            <div style="display:block;width:100%;height:50%;float:left;">
              <div style="display:block;width:20%;height:50%;float:left;"></div>
              <div style="display:block;width:20%;height:50%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustLightOff(-1)" />
              </div>
              <div style="display:block;width:20%;height:50%;float:left;"></div>
              <div style="display:block;width:20%;height:50%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustLightOff(1)" />
              </div>
              <div style="display:block;width:20%;height:50%;float:left;"></div>
            </div>
          </div>
          <div style="display:block;width:33.3%;height:60%;float:left;">
            <div style="display:block;width:20%;height:100%;float:left;"></div>
            <div style="display:block;width:60%;height:30%;float:left;margin-top:15%;">
              <div style="display:block;width:100%;height:50%;float:left;">
                <div id="lightOff" style="display:block;width:31%;height:100%;float:left;background:red;border-radius:8px 0px 0px 8px;border:1px solid;cursor:pointer;" onclick="adjustLightMode(2)"></div>
                <div id="lightAuto" style="display:block;width:31%;height:100%;float:left;background:red;border:1px solid;cursor:pointer;" onclick="adjustLightMode(0)"></div>
                <div id="lightOn" style="display:block;width:31%;height:100%;float:left;background:lime;border-radius:0px 8px 8px 0px;border:1px solid;cursor:pointer;" onclick="adjustLightMode(1)"></div>
              </div>
            </div>
            <div style="display:block;width:20%;height:100%;float:left;"></div>
          </div>
        </div>
      </div>
      <div id="page12" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;">
          <div class="slideIcon" style="float:left;padding-left:10px;padding-right:2px;">
            <img id="fanSlideIcon1"  style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="fanSlideIcon2" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="fanSlideIcon3" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="fanSlideIcon4" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="fanSlideIcon5" style="width:100%;height:100%;" />
          </div>
        </div>
        <div id="leftSlideArrow" onclick="fanPageSlider(-1)">
          <img class="leftSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
        <div id="rightSlideArrow" onclick="fanPageSlider(1)">
          <img class="rightSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
        <div style="width:94%;height:100%;float:left;">
          <div id="fanPage1" style="display:block;width:100%;height:100%;float:left;">
            <div style="display:block;width:100%;height:20%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Fan one target</p></b>
            </div>
            <div style="display:block;width:100%;height:20%;float:left;">
              <div style="display:block;width:50%;height:100%;float:left;">
                <b><p style="margin:0px;text-align:center;" class="p3text">Min speed</p></b>
              </div>
              <div style="display:block;width:50%;height:100%;float:left;">
                <b><p style="margin:0px;text-align:center;" class="p3text">Max speed</p></b>
              </div>
            </div>
            <div style="display:block;width:50%;height:60%;float:left;">
              <div style="display:block;width:100%;height:50%;float:left;">
                <center>
                  <b><p id="fanOneMinSpeed" class="p5text" style="margin:0px;display:inline;">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">%</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:50%;float:left;">
                <div style="display:block;width:15%;height:50%;float:left;"></div>
                <div style="display:block;width:25%;height:55%;float:left;">
                  <img class="ldbutt" alt="up arrow" onclick="adjustFanOneMinSpeed(-1)" style="float:right;"/>
                </div>
                <div style="display:block;width:20%;height:50%;float:left;"></div>
                <div style="display:block;width:25%;height:55%;float:left;">
                  <img class="lubutt" alt="down arrow" onclick="adjustFanOneMinSpeed(1)" style="float:left;"/>
                </div>
                <div style="display:block;width:15%;height:50%;float:left;"></div>
              </div>
            </div>
            <div style="display:block;width:50%;height:60%;float:left;">
              <div style="display:block;width:100%;height:50%;float:left;">
                <center>
                  <b><p id="fanOneMaxSpeed" class="p5text" style="margin:0px;display:inline;">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">%</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:50%;float:left;">
                <div style="display:block;width:15%;height:50%;float:left;"></div>
                <div style="display:block;width:25%;height:55%;float:left;">
                  <img class="ldbutt" alt="up arrow" onclick="adjustFanOneMaxSpeed(-1)" style="float:right;"/>
                </div>
                <div style="display:block;width:20%;height:50%;float:left;"></div>
                <div style="display:block;width:25%;height:55%;float:left;">
                  <img class="lubutt" alt="down arrow" onclick="adjustFanOneMaxSpeed(1)" style="float:left;"/>
                </div>
                <div style="display:block;width:15%;height:50%;float:left;"></div>
              </div>
            </div>
          </div>
          <div id="fanPage2" style="display:none;width:100%;height:100%;float:left;">
            <div style="display:block;width:100%;height:20%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Fan two target</p> </b>
            </div>
            <div style="display:block;width:100%;height:20%;float:left;">
              <div style="display:block;width:50%;height:100%;float:left;">
                <b><p style="margin:0px;text-align:center;" class="p3text">Min speed</p></b>
              </div>
              <div style="display:block;width:50%;height:100%;float:left;">
                <b><p style="margin:0px;text-align:center;" class="p3text">Max speed</p></b>
              </div>
            </div>
            <div style="display:block;width:50%;height:60%;float:left;">
              <div style="display:block;width:100%;height:50%;float:left;">
                <center>
                  <b><p id="fanTwoMinSpeed" class="p5text" style="margin:0px;display:inline;">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">%</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:50%;float:left;">
                <div style="display:block;width:15%;height:50%;float:left;"></div>
                <div style="display:block;width:25%;height:55%;float:left;">
                  <img class="ldbutt" alt="up arrow" onclick="adjustFanTwoMinSpeed(-1)" style="float:right;"/>
                </div>
                <div style="display:block;width:20%;height:50%;float:left;"></div>
                <div style="display:block;width:25%;height:55%;float:left;">
                  <img class="lubutt" alt="down arrow" onclick="adjustFanTwoMinSpeed(1)" style="float:left;"/>
                </div>
                <div style="display:block;width:15%;height:50%;float:left;"></div>
              </div>
            </div>
            <div style="display:block;width:50%;height:60%;float:left;">
              <div style="display:block;width:100%;height:50%;float:left;">
                <center>
                  <b><p id="fanTwoMaxSpeed" class="p5text" style="margin:0px;display:inline;">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">%</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:50%;float:left;">
                <div style="display:block;width:15%;height:50%;float:left;"></div>
                <div style="display:block;width:25%;height:55%;float:left;">
                  <img class="ldbutt" alt="up arrow" onclick="adjustFanTwoMaxSpeed(-1)" style="float:right;"/>
                </div>
                <div style="display:block;width:20%;height:50%;float:left;"></div>
                <div style="display:block;width:25%;height:55%;float:left;">
                  <img class="lubutt" alt="down arrow" onclick="adjustFanTwoMaxSpeed(1)" style="float:left;"/>
                </div>
                <div style="display:block;width:15%;height:50%;float:left;"></div>
              </div>
            </div>
          </div>
          <div id="fanPage3" style="display:none;width:100%;height:100%;float:left;">
            <div style="display:block;width:100%;height:20%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Target air temp</p></b>
            </div>
            <div style="display:block;width:100%;height:20%;float:left;">
              <div style="display:block;width:33.3%;height:100%;float:left;">
                <b><p style="margin:0px;text-align:center;" class="p3text">Minimum</p></b>
              </div>
              <div style="display:block;width:33.3%;height:100%;float:left;">
                <b><p style="margin:0px;text-align:center;" class="p3text">Maximum</p></b>
              </div>
              <div style="display:block;width:33.3%;height:100%;float:left;">
                <b><p style="margin:0px;text-align:center;" class="p3text">Tolerance</p></b>
              </div>
            </div>
            <div style="display:block;width:33.3%;height:60%;float:left;">
              <div style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
                <center>
                  <b><p id="minAirTemp" class="p5text" style="margin:0px;display:inline;">-</p></b><b><p name="cf" id="targetCo2Symbol" class="p3text" style="margin:0px;display:inline;">c</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:50%;float:left;">
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="ldbutt" alt="up arrow" onclick="adjustMinAirTemp(-0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="lubutt" alt="down arrow" onclick="adjustMinAirTemp(0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
              </div>
            </div>
            <div style="display:block;width:33.3%;height:60%;float:left;">
              <div style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
                <center>
                  <b><p id="maxAirTemp" class="p5text" style="margin:0px;display:inline;">-</p></b><b><p name="cf" class="p3text" style="margin:0px;display:inline;">c</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:50%;float:left;">
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="ldbutt" alt="up arrow" onclick="adjustMaxAirTemp(-0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="lubutt" alt="down arrow" onclick="adjustMaxAirTemp(0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
              </div>
            </div>
            <div style="display:block;width:33.3%;height:60%;float:left;">
              <div style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
                <center>
                  <b><p id="airTempOffset" class="p5text" style="margin:0px;display:inline;">-</p></b><b><p name="cf" class="p3text" style="margin:0px;display:inline;">c</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:50%;float:left;">
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="ldbutt" alt="up arrow" onclick="adjustAirTempOffset(-0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="lubutt" alt="down arrow" onclick="adjustAirTempOffset(0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
              </div>
            </div>
          </div>
          <div id="fanPage4" style="display:none;width:100%;height:100%;float:left;">
            <div style="display:block;width:100%;height:20%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Target humidity</p></b>
            </div>
            <div style="display:block;width:100%;height:20%;float:left;">
              <div style="display:block;width:33.3%;height:100%;float:left;">
                <b><p style="margin:0px;text-align:center;" class="p3text">Minimum</p></b>
              </div>
              <div style="display:block;width:33.3%;height:100%;float:left;">
                <b><p style="margin:0px;text-align:center;" class="p3text">Maximum</p></b>
              </div>
              <div style="display:block;width:33.3%;height:100%;float:left;">
                <b><p style="margin:0px;text-align:center;" class="p3text">Tolerance</p></b>
              </div>
            </div>
            <div style="display:block;width:33.3%;height:60%;float:left;">
              <div style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
                <center>
                  <b><p id="minHumidity" class="p5text" style="margin:0px;display:inline;">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">%</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:50%;float:left;">
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="ldbutt" alt="up arrow" onclick="adjustMinHumidity(-0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="lubutt" alt="down arrow" onclick="adjustMinHumidity(0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
              </div>
            </div>
            <div style="display:block;width:33.3%;height:60%;float:left;">
              <div style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
                <center>
                  <b><p id="maxHumidity" class="p5text" style="margin:0px;display:inline;">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">%</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:50%;float:left;">
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="ldbutt" alt="up arrow" onclick="adjustMaxHumidity(-0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="lubutt" alt="down arrow" onclick="adjustMaxHumidity(0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
              </div>
            </div>
            <div style="display:block;width:33.3%;height:60%;float:left;">
              <div style="display:block;width:100%;height:45%;float:left;align-items:baseline;">
                <center>
                  <b><p id="humidityOffset" class="p5text" style="margin:0px;display:inline;">-</p></b><b><p class="p3text" style="margin:0px;display:inline;">%</p></b>
                </center>
              </div>
              <div style="display:block;width:100%;height:50%;float:left;">
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="ldbutt" alt="up arrow" onclick="adjustHumidityOffset(-0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
                <div style="display:block;width:20%;height:55%;float:left;">
                  <img class="lubutt" alt="down arrow" onclick="adjustHumidityOffset(0.1)" />
                </div>
                <div style="display:block;width:20%;height:55%;float:left;"></div>
              </div>
            </div>
          </div>
          <div id="fanPage5" style="display:none;width:100%;height:100%;float:left;">
            <div style="display:block;width:100%;height:20%;float:left;">
              <b><p style="margin:0px;text-align:center;" class="p3text">Fan settings</p></b>
            </div>
            <div style="display:block;width:100%;height:70%;float:left;">
              <div style="display:block;width:100%;height:25%;float:left;">
                <div style="display:block;width:60%;height:100%;float:left;">
                  <b><p style="margin:0px;text-align:left;padding-left:5%;" class="p3text">Control temperature</p></b>
                </div>
                <div style="display:block;width:40%;height:100%;float:left;">
                  <div style="display:block;width:30%;height:100%;float:left;"></div>
                  <div style="display:block;width:50%;height:100%;float:right;">
                    <div style="display:block;width:100%;height:50%;float:right;">
                      <div
                        id="controlTempRight"
                        class="leftToggle"
                        onclick="adjustControlTemp(false)">
                      </div>
                      <div
                        id="controlTempLeft"
                        class="rightToggle"
                        onclick="adjustControlTemp(true)">
                      </div>
                    </div>
                  </div>
                  <div style="display:block;width:20%;height:100%;float:left;"></div>
                </div>
              </div>
              <div style="display:block;width:100%;height:25%;float:left;">
                <div style="display:block;width:60%;height:100%;float:left;">
                  <b><p style="margin:0px;text-align:left;padding-left:5%;" class="p3text">Control humidity</p></b>
                </div>
                <div style="display:block;width:40%;height:100%;float:left;">
                  <div style="display:block;width:30%;height:100%;float:left;"></div>
                  <div style="display:block;width:50%;height:100%;float:right;">
                    <div style="display:block;width:100%;height:50%;float:right;">
                      <div
                        id="controlHumRight"
                        class="leftToggle"
                        onclick="adjustControlHumidity(false)"></div>
                      <div
                        id="controlHumLeft"
                        class="rightToggle"
                        onclick="adjustControlHumidity(true)">
                      </div>
                    </div>
                  </div>
                  <div style="display:block;width:20%;height:100%;float:left;"></div>
                </div>
              </div>
              <div style="display:block;width:100%;height:25%;float:left;">
                <div style="display:block;width:60%;height:100%;float:left;">
                  <b><p style="margin:0px;text-align:left;padding-left:5%;" class="p3text">Fan one fixed speed</p></b>
                </div>
                <div style="display:block;width:40%;height:100%;float:left;">
                  <div style="display:block;width:30%;height:100%;float:left;"></div>
                  <div style="display:block;width:50%;height:100%;float:right;">
                    <div style="display:block;width:100%;height:50%;float:right;">
                      <div
                        id="fanOneFixedSpeedRight"
                        class="leftToggle"
                        onclick="adjustFanOneFixedSpeed(false)">
                      </div>
                      <div
                        id="fanOneFixedSpeedLeft"
                        class="rightToggle"
                        onclick="adjustFanOneFixedSpeed(true)">
                      </div>
                    </div>
                  </div>
                  <div style="display:block;width:20%;height:100%;float:left;"></div>
                </div>
              </div>
              <div style="display:block;width:100%;height:25%;float:left;">
                <div style="display:block;width:60%;height:100%;float:left;">
                  <b><p style="margin:0px;text-align:left;padding-left:5%;" class="p3text">Fan two fixed speed</p></b>
                </div>
                <div style="display:block;width:40%;height:100%;float:left;">
                  <div style="display:block;width:30%;height:100%;float:left;"></div>
                  <div style="display:block;width:50%;height:100%;float:right;">
                    <div style="display:block;width:100%;height:50%;float:right;">
                      <div
                        id="fanTwoFixedSpeedRight"
                        class="leftToggle"
                        onclick="adjustFanTwoFixedSpeed(false)">
                      </div>
                      <div
                        id="fanTwoFixedSpeedLeft"
                        class="rightToggle"
                        onclick="adjustFanTwoFixedSpeed(true)">
                      </div>
                    </div>
                  </div>
                  <div style="display:block;width:20%;height:100%;float:left;"></div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
      <div id="page13" style="display:none;width:100%;height:100%;">
        <div id="topbar" style="display:block;width:100%;">
          <div class="slideIcon" style="float:left;padding-left:10px;padding-right:2px;">
            <img id="warningSlideIcon1"  style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="warningSlideIcon2" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="warningSlideIcon3" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;">
            <img id="warningSlideIcon4" style="width:100%;height:100%;" />
          </div>
          <div class="slideIcon" style="float:left;padding-right:2px;"><img id="warningSlideIcon5" style="width:100%;height:100%;" /></div>
          <div class="slideIcon" style="float:left;padding-right:2px;"><img id="warningSlideIcon6" style="width:100%;height:100%;" /></div>
          <div class="slideIcon" style="float:left;padding-right:2px;"><img id="warningSlideIcon7" style="width:100%;height:100%;" /></div>
          <div class="slideIcon" style="float:left;padding-right:2px;"><img id="warningSlideIcon8" style="width:100%;height:100%;" /></div>
        </div>
        <div style="display:block;width:100%;height:20%;float:left;">
          <b><p class="p3text" id="warningTitle" style="margin:0px;text-align:center;">EC warning</p></b>
        </div>
        <div style="display:block;width:100%;height:20%;float:left;">
          <div style="display:block;width:50%;height:100%;float:left;">
            <b><p class="p3text" style="margin:0px;text-align:center;">Sensor reading</p></b>
          </div>
          <div style="display:block;width:50%;height:100%;float:left;">
            <b><p class="p3text" style="margin:0px;text-align:center;">Error margin +/-</p></b>
          </div>
        </div>
        <div id="maxMins" style="display:block;width:100%;height:50%;float:left;">
          <div style="display:block;width:50%;height:100%;float:left;">
            <div id="sensorWarningDiv" style="display:block;width:100%;height:50%;display:flex;justify-content:center;align-items:initial;">
              <b><p id="sensorWarning" class="p5text" style="margin:0px;display:inline;">-</p></b>
              <b><p id="warningSensorIcon" class="p3text" style="margin:0px;display:inline;"></p></b>
            </div>
            <div id="sensorWarningDiv" style="display:block;width:100%;height:50%;display:flex;justify-content:center;align-items:initial;"></div>
          </div>
          <div style="display:block;width:50%;height:100%;float:left;">
            <div id="warningMarginDiv" style="display:block;width:100%;height:51%;display:flex;justify-content:center;align-items:initial;">
              <b><p id="warningMargin" class="p5text" style="margin:0px;display:inline;">-</p></b>
              <b><p id="warningMarginIcon" class="p3text" style="margin:0px;display:inline;"></p></b>
            </div>
            <div id="sensorWarningDiv" style="display:block;width:100%;height:49%;display:flex;justify-content:center;align-items:initial;">
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="ldbutt" alt="up arrow" onclick="adjustWarningMargin(-1)" style="float:right;"/>
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
              <div style="display:block;width:20%;height:65%;float:left;">
                <img class="lubutt" alt="down arrow" onclick="adjustWarningMargin(1)" style="float:left;"/>
              </div>
              <div style="display:block;width:20%;height:65%;float:left;"></div>
            </div>
          </div>
        </div>
        <div id="leftSlideArrow" onclick="warningPageSlider(-1)">
          <img class="leftSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" />
        </div>
        <div id="rightSlideArrow" onclick="warningPageSlider(1)"><img class="rightSlideArrowIcon"  alt="right scroll" style="width:100%;height:100%;cursor:pointer;" /></div>
      </div>
    </div>
  </div>

  <!-- bottom section. -->
  <div class="bottomSection">
    <div style="width:10%;height:100%;float:left;"></div>
    <div class="bottomLeft">
      <div id="led" class="led-blue"></div>
    </div>
    <div class="bottomMiddle">
      <h1 id="lable">Hydro Controller</h1>
    </div>
  </div>
</div>
<footer>
  <p style="margin:0px;position:fixed;right:0;bottom:0;text-align:right;font-size:0.5vw;"><b>V1.3.2 last updated 20 March 2022&nbsp;</b></p>
</footer>
</div>
<script type="text/JavaScript">
var homePageTimer;
document.getElementById('profileInputBox_1').addEventListener('keydown', function(e) {
  const regex = RegExp('[0-9a-zA-Z_]');
  if (!regex.test(e.key) && e.key != 'backspace')
    e.preventDefault();
});
document.getElementById('profileInputBox_2').addEventListener('keydown', function(e) {
  const regex = RegExp('[0-9a-zA-Z_]');
  if (!regex.test(e.key) && e.key != 'backspace') 
    e.preventDefault();
});
document.getElementById('profileInputBox_3').addEventListener('keydown', function(e) {
  const regex = RegExp('[0-9a-zA-Z_]');
  if (!regex.test(e.key) && e.key != 'backspace')
    e.preventDefault();
});
document.getElementById('profileInputBox_4').addEventListener('keydown', function(e) {
  const regex = RegExp('[0-9a-zA-Z_]');
  if (!regex.test(e.key) && e.key != 'backspace')
    e.preventDefault();
});
document.getElementById('profileInputBox_5').addEventListener('keydown', function(e) {
  const regex = RegExp('[0-9a-zA-Z_]');
  if (!regex.test(e.key) && e.key != 'backspace')
    e.preventDefault();
});

function setLedColor() {
  var errorState = warningState.NO_WARNING;
  if (dosingInt > 0) {
    if (convertToTds && !disablePpmWarn) {
      if (tdsSensor <= (targetMinTds - tdsOffset - tdsErrorMargin) || tdsSensor >= (targetMaxTds + tdsOffset + tdsErrorMargin)) {
        errorState = warningState.MAJOR_WARNING;
        ppmErrorState = warningState.MAJOR_WARNING;
        //console.log("TDS MAJOR WARNING");
      }
      else if (tdsSensor < targetMinTds || tds > targetMaxTds) {
        if (errorState != warningState.MAJOR_WARNING)
          errorState = warningState.MINOR_WARNING;
        ppmErrorState = warningState.MINOR_WARNING;
        //console.log("TDS MINOR WARNING");
      }
      else {
        ppmErrorState = warningState.NO_WARNING;
      }
    }
    else {
      ppmErrorState = warningState.NO_WARNING;
    }
    //
    if (!convertToTds && !disableEcWarn) {
      if (ecSensor <= (targetMinEc - ecOffset - ecErrorMargin) || ecSensor >= (targetMaxEc + ecOffset + ecErrorMargin)) {
        errorState = warningState.MAJOR_WARNING;
        ecErrorState = warningState.MAJOR_WARNING;
        //console.log("EC MAJOR WARNING");
      }
      else if (ecSensor < targetMinEc || ecSensor > targetMaxEc) {
        if (errorState != warningState.MAJOR_WARNING)
          errorState = warningState.MINOR_WARNING;
        ecErrorState = warningState.MINOR_WARNING;
        //console.log("EC MINOR WARNING");
      }
      else {
        ecErrorState = warningState.NO_WARNING;
      }
    }
    else {
      ecErrorState = warningState.NO_WARNING;
    }
    //
    if (!disablePhWarn) {
      if (phSensor <= (targetMinPh - phOffset - phErrorMargin) || phSensor >= (targetMaxPh + phOffset + phErrorMargin)) {
        errorState = warningState.MAJOR_WARNING;
        phErrorState = warningState.MAJOR_WARNING;
        //console.log("PH MAJOR WARNING");
      }
      else if (phSensor < targetMinPh || phSensor > targetMaxPh) {
        if (errorState != warningState.MAJOR_WARNING)
          errorState = warningState.MINOR_WARNING;
        phErrorState = warningState.MINOR_WARNING;
        //console.log("PH MINOR WARNING");
      }
      else {
        phErrorState = warningState.NO_WARNING;
      }
    }
    else {
      phErrorState = warningState.NO_WARNING;
    }
    //
    if (!disableCo2Warn) {
      if (co2Sensor <= (targetCo2 - co2Offset - co2ErrorMargin)) {
        errorState = warningState.MAJOR_WARNING;
        co2ErrorState = warningState.MAJOR_WARNING;
        //console.log("CO2 MAJOR WARNING");
      }
      else if (co2Sensor < targetCo2) {
        if (errorState != warningState.MAJOR_WARNING)
          errorState = warningState.MINOR_WARNING;
        co2ErrorState = warningState.MINOR_WARNING;
        //console.log("CO2 MINOR WARNING");
      }
      else {
        co2ErrorState = warningState.NO_WARNING;
      }
    }
    else {
      co2ErrorState = warningState.NO_WARNING;
    }
    //
    if (!disableWaterTempWarn) {
      if (waterTemperatureSensor <= (targetMinWaterTemp - waterTempOffset - waterTempErrorMargin) || waterTemperatureSensor >= (targetMaxWaterTemp + waterTempOffset + waterTempErrorMargin)) {
        errorState = warningState.MAJOR_WARNING;
        waterTempErrorState = warningState.MAJOR_WARNING;
        //console.log("WATER TEMP MAJOR WARNING");
      }
      else if (waterTemperatureSensor < targetMinWaterTemp || waterTemperatureSensor > targetMaxWaterTemp) {
        if (errorState != warningState.MAJOR_WARNING)
          errorState = warningState.MINOR_WARNING;
        waterTempErrorState = warningState.MINOR_WARNING;
        //console.log("WATER TEMP MINOR WARNING");
      }
      else {
        waterTempErrorState = warningState.NO_WARNING;
      }
    }
    else {
      waterTempErrorState = warningState.NO_WARNING;
    }
    //
    if (!disableWaterHeightWarn) {
      if (waterLevelSensor <= (targetMinWaterHeight - waterHeightOffset - waterHeightErrorMargin) || waterLevelSensor >= (targetMaxWaterHeight + waterHeightOffset + waterHeightErrorMargin)) {
        errorState = warningState.MAJOR_WARNING;
        waterLevelErrorState = warningState.MAJOR_WARNING;
        //console.log("WATER HEIGHT MAJOR WARNING");
      }
      else if (waterLevelSensor < targetMinWaterHeight || waterLevelSensor > targetMaxWaterHeight) {
        if (errorState != warningState.MAJOR_WARNING)
          errorState = warningState.MINOR_WARNING;
          waterLevelErrorState = warningState.MINOR_WARNING;
          //console.log("WATER HEIGHT MINOR WARNING");
      }
      else {
        waterLevelErrorState = warningState.NO_WARNING;
      }
    }
    else {
      waterLevelErrorState = warningState.NO_WARNING;
    }
    //
    if (!disableAirTempWarn) {
      if (airTemperatureSensor <= (targetMinAirTemp - airTempOffset - airTempErrorMargin) || airTemperatureSensor >= (targetMaxAirTemp + airTempOffset + airTempErrorMargin)) {
        errorState = warningState.MAJOR_WARNING;
        airTempErrorState = warningState.MAJOR_WARNING;
        //console.log("AIR TEMP MAJOR WARNING");
      }
      else if (airTemperatureSensor < targetMinAirTemp || airTemperatureSensor > targetMaxAirTemp) {
        if (errorState != warningState.MAJOR_WARNING)
          errorState = warningState.MINOR_WARNING;
        airTempErrorState = warningState.MINOR_WARNING;
        //console.log("AIR TEMP MINOR WARNING");
      }
      else {
        airTempErrorState = warningState.NO_WARNING;
      }
    }
    else {
      airTempErrorState = warningState.NO_WARNING;
    }
    //
    if (!disableHumidityWarn) {
      if (airHumiditySensor <= (targetMinHumidity - humidityOffset - humidityErrorMargin) || airHumiditySensor >= (targetMaxHumidity + humidityOffset + humidityErrorMargin)) {
        errorState = warningState.MAJOR_WARNING;
        humidityErrorState = warningState.MAJOR_WARNING;
        //console.log("AIR HUMIDITY MAJOR WARNING");
      }
      else if (airHumiditySensor < targetMinHumidity || airHumiditySensor > targetMaxHumidity) {
        if (errorState != warningState.MAJOR_WARNING)
          errorState = warningState.MINOR_WARNING;
        humidityErrorState = warningState.MINOR_WARNING;
        //console.log("AIR HUMIDITY MINOR WARNING");
      }
      else {
        humidityErrorState = warningState.NO_WARNING;
      }
    }
    else {
      humidityErrorState = warningState.NO_WARNING;
    }
    //
    if (errorState != previousErrorState) {
      if (errorState == warningState.NO_WARNING) {
        document.getElementById("led").className = "led-blue";
      }
      else if (errorState == warningState.MINOR_WARNING) {
        document.getElementById("led").className = "led-yellow";
      }
      else if (errorState == warningState.MAJOR_WARNING) {
        document.getElementById("led").className = "led-red";
      }
      previousErrorState = errorState;
    }
  }
  clearTimeout(ledTimer);
  ledTimer = setTimeout(function() {
    setLedColor();
  }, 5000);
}

function setWarningTextColor(id, errorState) {
  var textColor = "black";
  if (errorState == warningState.MINOR_WARNING)
    textColor = "yellow";
  else if (errorState == warningState.MAJOR_WARNING)
    textColor = "red";
  document.getElementById(id).style.color = textColor;
}

function setPage(pageNumber, refreshValues) {
  guiPageNumber = pageNumber;
  var buttonID = "button" + pageNumber;
  var menuIcons = ["home", "graph", "maxMins", "presets", "cogs", "ec", "ph", "co2", "waterTemp", "doser", "light", "fan", "warning"];
  for (i = 1;i < 14;i++) {
    var currentButton = "button" + i;
    var currentButtonImg = "button" + i + "_Img";
    var currentPage = "page" + i;
    if (currentButton == buttonID) {
      document.getElementById(currentButtonImg).style.filter = "grayscale(0%)";
      document.getElementById(currentPage).style.display = "block";
    } 
    else {
      document.getElementById(currentButtonImg).style.filter = "grayscale(100%)";
      document.getElementById(currentPage).style.display = "none";
    }
  }
  if (pageNumber == 1) {
    homePageSlider(0);
  }
  else if (pageNumber == 2) {
    graphSlider(0);
  }
  else if (pageNumber == 3) {
    maxMinPageSlider(0);
  }
  else if (pageNumber == 4) {
    document.getElementById('profileInputBox_1').value = profileOneName;
    document.getElementById('profileInputBox_2').value = profileTwoName;
    document.getElementById('profileInputBox_3').value = profileThreeName;
    document.getElementById('profileInputBox_4').value = profileFourName;
    document.getElementById('profileInputBox_5').value = profileFiveName;
    for (var i = 0; i < 5; i++) {
      document.getElementById('profileInputBox_' + (i + 1)).style.border = (userProfile == i ? "medium solid #0000FF" : "none");
    }
  }
  else if (pageNumber == 5) {
    setToggleButton(convertToF,"tempInF");
    setToggleButton(convertToInchs,"heightInInchs");
    setToggleButton(convertToTds,"ecToTds");
    setToggleButton(disableCo2Warn,"disableCo2Warn");
    setToggleButton(disableEcWarn,"disableEcWarn");
    setToggleButton(disablePpmWarn,"disablePpmWarn");
    setToggleButton(disablePhWarn,"disablePhWarn");
    setToggleButton(disableWaterTempWarn,"disableWaterTempWarn");
    setToggleButton(disableWaterHeightWarn,"disableWaterHeightWarn");
    setToggleButton(disableAirTempWarn,"disableAirTempWarn");
    setToggleButton(disableHumidityWarn,"disableHumidityWarn");
    adjustnumDosers(null);
    adjustdosingInt(null);
  }
  else if (pageNumber == 6) {
    updateEcIcons();
  }
  else if (pageNumber == 7) {
    adjustMinPh(null);
    adjustMaxPh(null);
    adjustPhOffset(null);
  }
  else if (pageNumber == 8) {
    co2PageSlider(null);
    adjustTargetCo2(null);
    adjustTargetCo2Offset(null);
    adjustCo2RoomLength(null);
    adjustCo2RoomWidth(null);
    adjustCo2RoomHeight(null);
    adjustCo2FlowRate(null);
    adjustCo2Interval(null);
    adjustCo2FanDisableTime(null);
    adjustManualGasDurationTime(null);
    setToggleButton(manGasTimeEnabled,"gasTimeEnabled");
    setToggleButton(co2ControlDisabled,"disableCo2Control");
  }
  else if (pageNumber == 9) {
    waterPageSlider(0);
  }
  else if (pageNumber == 10) {
    doserPageSlider(0);
  }
  else if (pageNumber == 11) {
    if (lightMode == 0) {
      document.getElementById("lightOn").style.backgroundColor = "lightgrey";
      document.getElementById("lightAuto").style.backgroundColor = "lime";
      document.getElementById("lightOff").style.backgroundColor = "lightgrey";
    }
    else if (lightMode == 1) {
      document.getElementById("lightOn").style.backgroundColor = "lime";
      document.getElementById("lightAuto").style.backgroundColor = "lightgrey";
      document.getElementById("lightOff").style.backgroundColor = "lightgrey";
    }
    else if (lightMode == 2) {
      document.getElementById("lightOn").style.backgroundColor = "lightgrey";
      document.getElementById("lightAuto").style.backgroundColor = "lightgrey";
      document.getElementById("lightOff").style.backgroundColor = "red";
    }
    adjustLightOn(null);
    adjustLightOff(null);
  }
  else if (pageNumber == 12) {
    fanPageSlider(0);
    adjustMinAirTemp(null);
    adjustMaxAirTemp(null);
    adjustAirTempOffset(null);
    adjustFanOneMinSpeed(null);
    adjustFanOneMaxSpeed(null);
    adjustFanTwoMinSpeed(null);
    adjustFanTwoMaxSpeed(null);
    adjustMinHumidity(null);
    adjustMaxHumidity(null);
    adjustHumidityOffset(null);
    setToggleButton(fansControlTemp,"controlTemp");
    setToggleButton(fansControlHum,"controlHum");
    setToggleButton(fanOneFixedSpeed,"fanOneFixedSpeed");
    setToggleButton(fanTwoFixedSpeed,"fanTwoFixedSpeed");
  }
  else if (pageNumber == 13) {
    warningPageSlider(0);
  }
}

function updateCfIcons() {
  var cfIcons = document.getElementsByName("cf");
  for (var i = 0;i < cfIcons.length;i++) {
    if (convertToF) {
      cfIcons[i].innerHTML = "f";
    }
    else {
      cfIcons[i].innerHTML = "c";
    }
  }
}

function updateCmIcons() {
  var cmIcons = document.getElementsByName("cm");
  for (var i = 0;i < cmIcons.length;i++) {
    if (convertToInchs) {
      cmIcons[i].innerHTML = "\"";
      cmIcons[i].style.verticalAlign = "2vw";
    }
    else {
      cmIcons[i].innerHTML = "cm";
      cmIcons[i].style.verticalAlign = "bottom";
    }
  }
  var floawRateIcon = document.getElementById("co2FlowRateSymbol");
  if (convertToInchs) {
    floawRateIcon.innerHTML = "ft3";
    floawRateIcon.style.verticalAlign = "1.6vw";
  }
  else {
    floawRateIcon.innerHTML = "ltr";
    floawRateIcon.style.verticalAlign = "bottom";
  }
}

function updateEcIcons() {
  if(convertToTds) {
    document.getElementById("button6_Img").src=document.getElementById("pm_blue").src;
    document.getElementById('TargetEcTitle').innerHTML = "Target TDS";
  }
  else {
    document.getElementById("button6_Img").src=document.getElementById("ec_blue").src;
    document.getElementById('TargetEcTitle').innerHTML = "Target EC"; 
  }
  adjustMinEc(null);
  adjustMaxEc(null);
  adjustEcOffset(null);
}

function homePageSlider(inc) {
  if (inc < 0) {
    if (homePageScrollPosition > 0) homePageScrollPosition--;
    else homePageScrollPosition = 9;
  }
  else if (inc > 0) {
    if (homePageScrollPosition < 9) homePageScrollPosition++;
    else homePageScrollPosition = 0;
  }

  var currentSlideIcon = "homeSlideIcon" + (homePageScrollPosition + 1);
  //console.log(currentSlideIcon);
  for (i = 1; i < 11;i++) {
    var slideIcon = "homeSlideIcon" + i;
    document.getElementById(slideIcon).src = document.getElementById("slideIcon_blue").src;
    if (slideIcon == currentSlideIcon) {
      document.getElementById(slideIcon).style.filter = "grayscale(0%)";
    }
    else {
      document.getElementById(slideIcon).style.filter = "grayscale(100%)";
    }
  }

  let box = document.getElementById("homePageGraph");

  if (homePageScrollPosition == 0) {
    document.getElementById('homePageSensorTitle').innerHTML = "EC Sensor";
    document.getElementById('homePageIcon').innerHTML = "";
    document.getElementById("homeSensorDiv").style.alignItems = "baseline";
    document.getElementById('homePageSensorValue').innerHTML = parseFloat(ecSensor).toFixed(2);
    document.getElementById('homePageTargetTitle').innerHTML = "Target +/-" + parseFloat(ecOffset).toFixed(2);
    document.getElementById('homePageMinTargetValue').innerHTML = parseFloat(targetMinEc).toFixed(2);
    document.getElementById('homePageMaxTargetValue').innerHTML = parseFloat(targetMaxEc).toFixed(2);
    drawGraph("graphCanvas",2,3,box.offsetHeight-5,box.offsetWidth-2,10,true,ecSenorArray,arrayCount,1);
  }
  else if (homePageScrollPosition == 1) {
    document.getElementById('homePageSensorTitle').innerHTML = "TDS Sensor";
    document.getElementById('homePageIcon').innerHTML = "";
    document.getElementById("homeSensorDiv").style.alignItems = "baseline";
    document.getElementById('homePageSensorValue').innerHTML = tdsSensor;
    document.getElementById('homePageTargetTitle').innerHTML = "Target +/-" + tdsOffset;
    document.getElementById('homePageMinTargetValue').innerHTML = targetMinTds;
    document.getElementById('homePageMaxTargetValue').innerHTML = targetMaxTds;
    drawGraph("graphCanvas",2,3,box.offsetHeight-5,box.offsetWidth-2,10,true,tdsSenorArray,arrayCount,1);
  }
  else if (homePageScrollPosition == 2) {
    document.getElementById('homePageSensorTitle').innerHTML = "PH Sensor";
    document.getElementById('homePageIcon').innerHTML = "";
    document.getElementById("homeSensorDiv").style.alignItems = "baseline";
    document.getElementById('homePageSensorValue').innerHTML = parseFloat(phSensor).toFixed(2);
    document.getElementById('homePageTargetTitle').innerHTML = "Target +/-" + parseFloat(phOffset).toFixed(2);
    document.getElementById('homePageMinTargetValue').innerHTML = parseFloat(targetMinPh).toFixed(2);
    document.getElementById('homePageMaxTargetValue').innerHTML = parseFloat(targetMaxPh).toFixed(2);
    drawGraph("graphCanvas",2,3,box.offsetHeight-5,box.offsetWidth-2,10,true,phSenorArray,arrayCount,1);
  }
  else if (homePageScrollPosition == 3) {
    document.getElementById('homePageSensorTitle').innerHTML = "Co2 Sensor";
    document.getElementById('homePageIcon').innerHTML = "";
    document.getElementById("homeSensorDiv").style.alignItems = "baseline";
    document.getElementById('homePageSensorValue').innerHTML = co2Sensor;
    document.getElementById('homePageTargetTitle').innerHTML = "Target";
    document.getElementById('homePageMinTargetValue').innerHTML = targetCo2;
    document.getElementById('homePageMaxTargetValue').innerHTML = targetCo2;
    drawGraph("graphCanvas",2,3,box.offsetHeight-5,box.offsetWidth-2,10,true,co2SenorArray,arrayCount,1);
  }
  else if (homePageScrollPosition == 4) {
    document.getElementById('homePageSensorTitle').innerHTML = "Water Temp Sensor";
    document.getElementById("homeSensorDiv").style.alignItems = "initial";
    document.getElementById('homePageIcon').style.verticalAlign = "top";
    if (convertToF) {
      document.getElementById('homePageIcon').innerHTML = "f";
    }
    else {
      document.getElementById('homePageIcon').innerHTML = "c";      
    }
    drawGraph("graphCanvas",2,3,box.offsetHeight-5,box.offsetWidth-2,10,true,waterTemperatureSenorArray,arrayCount,1);
    document.getElementById('homePageSensorValue').innerHTML = parseFloat(waterTemperatureSensor).toFixed(1);
    document.getElementById('homePageTargetTitle').innerHTML = "Target +/-" + parseFloat(waterTempOffset).toFixed(1);
    document.getElementById('homePageMinTargetValue').innerHTML = parseFloat(targetMinWaterTemp).toFixed(1);
    document.getElementById('homePageMaxTargetValue').innerHTML = parseFloat(targetMaxWaterTemp).toFixed(1);
  }
  else if (homePageScrollPosition == 5) {
    document.getElementById('homePageSensorTitle').innerHTML = "Water Level Sensor";
    if (convertToInchs) {
      document.getElementById("homeSensorDiv").style.alignItems = "initial";
      document.getElementById('homePageIcon').innerHTML = "\"";
      document.getElementById('homePageIcon').style.verticalAlign = "top";    
    }
    else {
      document.getElementById("homeSensorDiv").style.alignItems = "baseline";
      document.getElementById('homePageIcon').innerHTML = "cm";
      document.getElementById('homePageIcon').style.verticalAlign = "text-bottom";
    }
    drawGraph("graphCanvas",2,3,box.offsetHeight-5,box.offsetWidth-2,10,true,waterLevelSenorArray,arrayCount,1);
    document.getElementById('homePageSensorValue').innerHTML = parseFloat(waterLevelSensor).toFixed(1);
    document.getElementById('homePageTargetTitle').innerHTML = "Target +/-" + parseFloat(waterHeightOffset).toFixed(1);
    document.getElementById('homePageMinTargetValue').innerHTML = parseFloat(targetMinWaterHeight).toFixed(1);
    document.getElementById('homePageMaxTargetValue').innerHTML = parseFloat(targetMaxWaterHeight).toFixed(1);
  }
  else if (homePageScrollPosition == 6) {
    document.getElementById('homePageSensorTitle').innerHTML = "Air Temp Sensor";
    document.getElementById("homeSensorDiv").style.alignItems = "initial";
    document.getElementById('homePageIcon').style.verticalAlign = "top";
    if (convertToF) {
      document.getElementById('homePageIcon').innerHTML = "f";      
    }
    else {
      document.getElementById('homePageIcon').innerHTML = "c";
    }
    drawGraph("graphCanvas",2,3,box.offsetHeight-5,box.offsetWidth-2,10,true,airTemperatureSenorArray,arrayCount,1);
    document.getElementById('homePageSensorValue').innerHTML = parseFloat(airTemperatureSensor).toFixed(1);
    document.getElementById('homePageTargetTitle').innerHTML = "Target +/-" + parseFloat(airTempOffset).toFixed(1);
    document.getElementById('homePageMinTargetValue').innerHTML = parseFloat(targetMinAirTemp).toFixed(1);
    document.getElementById('homePageMaxTargetValue').innerHTML = parseFloat(targetMaxAirTemp).toFixed(1);
  }
  else if (homePageScrollPosition == 7) {
    document.getElementById('homePageSensorTitle').innerHTML = "Air Hum Sensor";
    document.getElementById("homeSensorDiv").style.alignItems = "baseline";
    document.getElementById('homePageIcon').innerHTML = "%";
    document.getElementById('homePageIcon').style.verticalAlign = "text-bottom";
    document.getElementById('homePageSensorValue').innerHTML = parseFloat(airHumiditySensor).toFixed(1);
    document.getElementById('homePageTargetTitle').innerHTML = "Target +/-" + parseFloat(humidityOffset).toFixed(1);
    document.getElementById('homePageMinTargetValue').innerHTML = parseFloat(targetMinHumidity).toFixed(1);
    document.getElementById('homePageMaxTargetValue').innerHTML = parseFloat(targetMaxHumidity).toFixed(1);
    drawGraph("graphCanvas",2,3,box.offsetHeight-5,box.offsetWidth-2,10,true,airHumiditySenorArray,arrayCount,1);
  }
  else if (homePageScrollPosition == 8) {
    document.getElementById('homePageSensorTitle').innerHTML = "Fan One Speed";
    document.getElementById("homeSensorDiv").style.alignItems = "baseline";
    document.getElementById('homePageIcon').innerHTML = "%";
    document.getElementById('homePageIcon').style.verticalAlign = "text-bottom";
    document.getElementById('homePageSensorValue').innerHTML = fanOneSpeed;
    document.getElementById('homePageTargetTitle').innerHTML = "Target";
    document.getElementById('homePageMinTargetValue').innerHTML = fanOneMinSpeed;
    document.getElementById('homePageMaxTargetValue').innerHTML = fanOneMaxSpeed;
    drawGraph("graphCanvas",2,3,box.offsetHeight-5,box.offsetWidth-2,10,true,fanOneSpeedArray,arrayCount,0);
  }
  else if (homePageScrollPosition == 9) {
    document.getElementById('homePageSensorTitle').innerHTML = "Fan Two Speed";
    document.getElementById("homeSensorDiv").style.alignItems = "baseline";
    document.getElementById('homePageIcon').innerHTML = "%";
    document.getElementById('homePageIcon').style.verticalAlign = "text-bottom";
    document.getElementById('homePageSensorValue').innerHTML = fantwoSpeed;
    document.getElementById('homePageTargetTitle').innerHTML = "Target";
    document.getElementById('homePageMinTargetValue').innerHTML = fanTwoMinSpeed;
    document.getElementById('homePageMaxTargetValue').innerHTML = fanTwoMaxSpeed;
    drawGraph("graphCanvas",2,3,box.offsetHeight-5,box.offsetWidth-2,10,true,fanTwoSpeedArray,arrayCount,0);
  }
  clearTimeout(homePageTimer);
  homePageTimer = setTimeout(function() {
    homePageSlider(1);
  }, 10000);
}

function graphSlider(inc) {
  setGraph2Size();
  if (inc < 0) {
    if (p2SPos > 0) p2SPos--;
    else p2SPos = 9;
  }
  else if (inc > 0) {
    if (p2SPos < 9) p2SPos++;
    else p2SPos = 0;
  }

  var currentSlideIcon = "graphSlideIcon" + (p2SPos + 1);
  for (i = 1;i < 11;i++) {
    var slideIcon = "graphSlideIcon" + i;
    document.getElementById(slideIcon).src = document.getElementById("slideIcon_blue").src;
    if (slideIcon == currentSlideIcon) {
      document.getElementById(slideIcon).style.filter = "grayscale(0%)";
    } 
    else {
      document.getElementById(slideIcon).style.filter = "grayscale(100%)";
    }
  }
  
  let box2 = document.getElementById("graphPageGraph");
  if (p2SPos == 0) {
    document.getElementById('graphPageSensorTitle').innerHTML = "EC Sensor";
    drawGraph("graphCanvas2", 2,6,box2.offsetHeight-17,box2.offsetWidth-30,10,true,ecSenorArray,arrayCount,1);
  }
  else if (p2SPos == 1) {
    document.getElementById('graphPageSensorTitle').innerHTML = "TDS Sensor";
    drawGraph("graphCanvas2", 2,6,box2.offsetHeight-17,box2.offsetWidth-30,10,true,tdsSenorArray,arrayCount,0);
  }
  else if (p2SPos == 2) {
    document.getElementById('graphPageSensorTitle').innerHTML = "PH Sensor";
    drawGraph("graphCanvas2", 2,6,box2.offsetHeight-17,box2.offsetWidth-30,10,true,phSenorArray,arrayCount,1);
  }
  else if (p2SPos == 3) {
    document.getElementById('graphPageSensorTitle').innerHTML = "Co2 Sensor";
    drawGraph("graphCanvas2", 2,6,box2.offsetHeight-17,box2.offsetWidth-30,10,true,co2SenorArray,arrayCount,1);
  }
  else if (p2SPos == 4) {
    document.getElementById('graphPageSensorTitle').innerHTML = "Water Temp Sensor";
    drawGraph("graphCanvas2", 2,6,box2.offsetHeight-17,box2.offsetWidth-30,10,true,waterTemperatureSenorArray,arrayCount,1);
  }
  else if (p2SPos == 5) {
    document.getElementById('graphPageSensorTitle').innerHTML = "Water Level Sensor";
    drawGraph("graphCanvas2", 2,6,box2.offsetHeight-17,box2.offsetWidth-30,10,true,waterLevelSenorArray,arrayCount,1);
  }
  else if (p2SPos == 6) {
    document.getElementById('graphPageSensorTitle').innerHTML = "Air Temp Sensor";
    drawGraph("graphCanvas2", 2,6,box2.offsetHeight-17,box2.offsetWidth-30,10,true,airHumiditySenorArray,arrayCount,1);
  }
  else if (p2SPos == 7) {
    document.getElementById('graphPageSensorTitle').innerHTML = "Air Hum Sensor";
    drawGraph("graphCanvas2", 2,6,box2.offsetHeight-17,box2.offsetWidth-30,10,true,airHumiditySenorArray,arrayCount,1);
  }
  else if (p2SPos == 8) {
    document.getElementById('graphPageSensorTitle').innerHTML = "Fan One Speed";
    drawGraph("graphCanvas2", 2,6,box2.offsetHeight-17,box2.offsetWidth-30,10,true,fanOneSpeedArray,arrayCount,0);
  }
  else if (p2SPos == 9) {
    document.getElementById('graphPageSensorTitle').innerHTML = "Fan Two Speed";
    drawGraph("graphCanvas2", 2,6,box2.offsetHeight-17,box2.offsetWidth-30,10,true,fanTwoSpeedArray,arrayCount,0);
  }
}

function maxMinPageSlider(inc) {
  if (inc < 0) {
    if (maxMinPageScrollPosition > 0) maxMinPageScrollPosition--;
    else maxMinPageScrollPosition = 9;
  } 
  else if (inc > 0) {
    if (maxMinPageScrollPosition < 9) maxMinPageScrollPosition++;
    else maxMinPageScrollPosition = 0;
  }
  var currentSlideIcon = "maxMinSlideIcon" + (maxMinPageScrollPosition + 1);
  for (i = 1;i < 11;i++) {
    var slideIcon = "maxMinSlideIcon" + i;
    document.getElementById(slideIcon).src = document.getElementById("slideIcon_blue").src;
    if (slideIcon == currentSlideIcon) {
      document.getElementById(slideIcon).style.filter = "grayscale(0%)";
    } 
    else {
      document.getElementById(slideIcon).style.filter = "grayscale(100%)";
    }
  }
  if (maxMinPageScrollPosition == 0) {
    document.getElementById('maxMinPageTitle').innerHTML = "EC History";
    document.getElementById('minValue').innerHTML = parseFloat(minEc).toFixed(2);
    document.getElementById('maxValue').innerHTML = parseFloat(maxEc).toFixed(2);
    document.getElementById("maxMin_min").innerHTML = "";
    document.getElementById("maxMin_max").innerHTML = "";
    document.getElementById("minHistoryValue").style.alignItems = "initial";
    document.getElementById("maxHistoryValue").style.alignItems = "initial";
  }
  else if (maxMinPageScrollPosition == 1) {
    document.getElementById('maxMinPageTitle').innerHTML = "TDS History";
    document.getElementById('minValue').innerHTML = minTds;
    document.getElementById('maxValue').innerHTML = maxTds;
    document.getElementById("maxMin_min").innerHTML = "";
    document.getElementById("maxMin_max").innerHTML = "";
    document.getElementById("minHistoryValue").style.alignItems = "initial";
    document.getElementById("maxHistoryValue").style.alignItems = "initial";
  }
  else if (maxMinPageScrollPosition == 2) {
    document.getElementById('maxMinPageTitle').innerHTML = "PH History";
    document.getElementById('minValue').innerHTML = parseFloat(minPh).toFixed(2);
    document.getElementById('maxValue').innerHTML = parseFloat(maxPh).toFixed(2);
    document.getElementById("maxMin_min").innerHTML = "";
    document.getElementById("maxMin_max").innerHTML = "";
    document.getElementById("minHistoryValue").style.alignItems = "initial";
    document.getElementById("maxHistoryValue").style.alignItems = "initial";
  }
  else if (maxMinPageScrollPosition == 3) {
    document.getElementById('maxMinPageTitle').innerHTML = "Co2 History";
    document.getElementById('minValue').innerHTML = minCo2;
    document.getElementById('maxValue').innerHTML = maxCo2;
    document.getElementById("maxMin_min").innerHTML = "ppm";
    document.getElementById("maxMin_max").innerHTML = "ppm";
    document.getElementById("minHistoryValue").style.alignItems = "baseline";
    document.getElementById("maxHistoryValue").style.alignItems = "baseline";
  }
  else if (maxMinPageScrollPosition == 4) {
    document.getElementById('maxMinPageTitle').innerHTML = "Water Temp History";
    document.getElementById("minHistoryValue").style.alignItems = "initial";
    document.getElementById("maxHistoryValue").style.alignItems = "initial";
    document.getElementById('minValue').innerHTML = parseFloat(minWaterTemp).toFixed(1);
    document.getElementById('maxValue').innerHTML = parseFloat(maxWaterTemp).toFixed(1);
    if (convertToF) {
      document.getElementById("maxMin_min").innerHTML = "f";
      document.getElementById("maxMin_max").innerHTML = "f";
    } 
    else {
      document.getElementById("maxMin_min").innerHTML = "c";
      document.getElementById("maxMin_max").innerHTML = "c";
    }
  } 
  else if (maxMinPageScrollPosition == 5) {
    document.getElementById('maxMinPageTitle').innerHTML = "Water Level History";
    document.getElementById('minValue').innerHTML = parseFloat(minWaterLevel).toFixed(1);
    document.getElementById('maxValue').innerHTML = parseFloat(maxWaterLevel).toFixed(1);
    if (convertToInchs) {
      document.getElementById("minHistoryValue").style.alignItems = "initial";
      document.getElementById("maxHistoryValue").style.alignItems = "initial";
      document.getElementById("maxMin_min").innerHTML = "\"";
      document.getElementById("maxMin_max").innerHTML = "\"";
    }
    else {
      document.getElementById("minHistoryValue").style.alignItems = "baseline";
      document.getElementById("maxHistoryValue").style.alignItems = "baseline";
      document.getElementById("maxMin_min").innerHTML = "cm";
      document.getElementById("maxMin_max").innerHTML = "cm";
    }
  } 
  else if (maxMinPageScrollPosition == 6) {
    document.getElementById('maxMinPageTitle').innerHTML = "Air Temp History";
    document.getElementById("minHistoryValue").style.alignItems = "initial";
    document.getElementById("maxHistoryValue").style.alignItems = "initial";
    document.getElementById('minValue').innerHTML = parseFloat(minAirTemp).toFixed(1);
    document.getElementById('maxValue').innerHTML = parseFloat(maxAirTemp).toFixed(1);
    if (convertToF) {
      document.getElementById("maxMin_min").innerHTML = "f";
      document.getElementById("maxMin_max").innerHTML = "f";
    }
    else {
      document.getElementById("maxMin_min").innerHTML = "c";
      document.getElementById("maxMin_max").innerHTML = "c";
    }
  }
  else if (maxMinPageScrollPosition == 7) {
    document.getElementById('maxMinPageTitle').innerHTML = "Air Hum History";
    document.getElementById('minValue').innerHTML = parseFloat(minHumidity).toFixed(1);
    document.getElementById('maxValue').innerHTML = parseFloat(maxHumidity).toFixed(1);
    document.getElementById("minHistoryValue").style.alignItems = "baseline";
    document.getElementById("maxHistoryValue").style.alignItems = "baseline";
    document.getElementById("maxMin_min").innerHTML = "%";
    document.getElementById("maxMin_max").innerHTML = "%";
  }
  else if (maxMinPageScrollPosition == 8) {
    document.getElementById('maxMinPageTitle').innerHTML = "Fan One History";
    document.getElementById('minValue').innerHTML = minFanOneSpeed;
    document.getElementById('maxValue').innerHTML = maxFanOneSpeed;
    document.getElementById("minHistoryValue").style.alignItems = "baseline";
    document.getElementById("maxHistoryValue").style.alignItems = "baseline";
    document.getElementById("maxMin_min").innerHTML = "%";
    document.getElementById("maxMin_max").innerHTML = "%";
  }
  else if (maxMinPageScrollPosition == 9) {
    document.getElementById('maxMinPageTitle').innerHTML = "Fan Two History";
    document.getElementById('minValue').innerHTML = minFanTwoSpeed;
    document.getElementById('maxValue').innerHTML = maxFanTwoSpeed;
    document.getElementById("minHistoryValue").style.alignItems = "baseline";
    document.getElementById("maxHistoryValue").style.alignItems = "baseline";
    document.getElementById("maxMin_min").innerHTML = "%";
    document.getElementById("maxMin_max").innerHTML = "%";
  }
}

function co2PageSlider(inc) {
  if (inc < 0) {
    if (co2PageScrollPosition > 0) co2PageScrollPosition--;
    else co2PageScrollPosition = 3;
  }
  else if (inc > 0) {
    if (co2PageScrollPosition < 3) co2PageScrollPosition++;
    else co2PageScrollPosition = 0;
  }

  var currentSlideIcon = "co2SlideIcon" + (co2PageScrollPosition + 1);
  var currentPage = "co2page" + (co2PageScrollPosition + 1);
  //console.log(currentSlideIcon);
  for (i = 1;i < 5;i++) {
    var slideIcon = "co2SlideIcon" + i;
    var newPage = "co2page" + i;
    document.getElementById(slideIcon).src = document.getElementById("slideIcon_blue").src;
    if (slideIcon == currentSlideIcon) {
      document.getElementById(slideIcon).style.filter = "grayscale(0%)";
      document.getElementById(newPage).style.display = "block";
    }
    else {
      document.getElementById(slideIcon).style.filter = "grayscale(100%)";
      document.getElementById(newPage).style.display = "none";
    }
  }
  if (convertToInchs) {
    document.getElementById("minCo2Div").style.alignItems = "initial";
    document.getElementById("maxCo2Div").style.alignItems = "initial";
    document.getElementById("co2OffsetDiv").style.alignItems = "initial";
  }
  else {
    document.getElementById("minCo2Div").style.alignItems = "baseline";
    document.getElementById("maxCo2Div").style.alignItems = "baseline";
    document.getElementById("co2OffsetDiv").style.alignItems = "baseline";
  }
}

var previousWaterPageDoserScrollPosition = 0;

function waterPageSlider(inc) {
  var doserPages = numDosers - 4;
  if (inc < 0) {
    if (waterPageScrollPosition > 0 && waterPageDoserScrollPosition == 0)
        waterPageScrollPosition--;
    else if (waterPageDoserScrollPosition > 0)
        waterPageDoserScrollPosition--;
    else if (waterPageDoserScrollPosition == 0)
        waterPageScrollPosition = 3;
  }
  else if (inc > 0) {
    if (waterPageScrollPosition < 3)
        waterPageScrollPosition++;
    else if (waterPageDoserScrollPosition < doserPages)
        waterPageDoserScrollPosition++;
    else if (waterPageScrollPosition == 3) {
        waterPageScrollPosition = 0;
        waterPageDoserScrollPosition = 0;
    }
  }
  var currentSlideIcon = "waterSlideIcon" + (waterPageScrollPosition + 1);
  for (i = 1;i < 5;i++) {
    var slideIcon = "waterSlideIcon" + i;
    var newPage = "waterPage" + i;
    document.getElementById(slideIcon).src = document.getElementById("slideIcon_blue").src;
    if (slideIcon == currentSlideIcon) {
      document.getElementById(slideIcon).style.filter = "grayscale(0%)";
      document.getElementById(newPage).style.display = "block";
    }
    else {
      document.getElementById(slideIcon).style.filter = "grayscale(100%)";
      document.getElementById(newPage).style.display = "none";
    }
  }
  if (waterPageScrollPosition == 0) {
    document.getElementById("targetMinWaterLvl").innerHTML = parseFloat(targetMinWaterHeight).toFixed(1);
    document.getElementById("targetMaxWaterLvl").innerHTML = parseFloat(targetMaxWaterHeight).toFixed(1);
    document.getElementById("targetWaterLvlOffset").innerHTML = parseFloat(waterHeightOffset).toFixed(1);
    if (convertToInchs) {
      document.getElementById("targetMinWaterLvlDiv").style.alignItems = "initial";
      document.getElementById("targetMaxWaterLvlDiv").style.alignItems = "initial";
      document.getElementById("targetWaterLvlOffsetDiv").style.alignItems = "initial";
    }
    else {
      document.getElementById("targetMinWaterLvlDiv").style.alignItems = "baseline";
      document.getElementById("targetMaxWaterLvlDiv").style.alignItems = "baseline";
      document.getElementById("targetWaterLvlOffsetDiv").style.alignItems = "baseline";
    }
    var waterIcons = document.getElementsByName("waterIcon");
    for (var i = 0;i < waterIcons.length;i++) {
      if (convertToInchs) {
        waterIcons[i].innerHTML = "\"";
      }
      else {
        waterIcons[i].innerHTML = "cm";
      }
    }
  }
  else if (waterPageScrollPosition == 1) {
    document.getElementById("targetMinWaterTemp").innerHTML = parseFloat(targetMinWaterTemp).toFixed(1);
    document.getElementById("targetMaxWaterTemp").innerHTML = parseFloat(targetMaxWaterTemp).toFixed(1);
    document.getElementById("targetWaterTempOffset").innerHTML = parseFloat(waterTempOffset).toFixed(1);
    var waterIcons = document.getElementsByName("waterIcon");
    for (var i = 0;i < waterIcons.length;i++) {
      if (convertToF) {
        waterIcons[i].innerHTML = "f";
      }
      else {
        waterIcons[i].innerHTML = "c";
      }
    }
  }
  else if (waterPageScrollPosition == 2) {
    for (var i = 0;i < 31;i++) {
      var calanderDaysNumber = "calanderDay" + (i+1);
      if (parseInt(autoFillDays[i]) == 1)
        document.getElementById(calanderDaysNumber).style.backgroundColor = "#0066ff";
      else
        document.getElementById(calanderDaysNumber).style.backgroundColor = "white";
    }
    var timeSymbol = autoFillHour < 12 ? "AM" :"PM";
    var t_autoFillHour = autoFillHour;
    var t_autoFillMinute = autoFillMinute;
    if (t_autoFillHour < 10) t_autoFillHour = "0" + t_autoFillHour;
    if (t_autoFillMinute < 10) t_autoFillMinute = "0" + t_autoFillMinute;
    document.getElementById("refillTankTime").innerHTML = "Time " + t_autoFillHour + ":" + t_autoFillMinute + timeSymbol;
    if (disableDrainAndRefill) {
      document.getElementById("dRefillDrainLeft").style.backgroundColor = "lightgrey";
      document.getElementById("dRefillDrainRight").style.backgroundColor = "lime";
    }
    else {
      document.getElementById("dRefillDrainLeft").style.backgroundColor = "red";
      document.getElementById("dRefillDrainRight").style.backgroundColor = "lightgrey";
    }
  }
  else if (waterPageScrollPosition == 3) {
    if (waterPageDoserScrollPosition == 0) {
      document.getElementById("waterDoser1").innerHTML = "1";
      document.getElementById("waterDoser2").innerHTML = "2";
      document.getElementById("waterDoser3").innerHTML = "3";
      document.getElementById("waterDoser4").innerHTML = "4";
      if (doserOneMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton1").value = "OFF";
      }
      else if (doserOneMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton1").value = "EC";
      }
      else if (doserOneMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton1").value = "PH";
      }
      if (doserTwoMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton2").value = "OFF";
      }
      else if (doserTwoMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton2").value = "EC";
      }
      else if (doserTwoMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton2").value = "PH"
      }
      if (doserThreeMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton3").value = "OFF";
      }
      else if (doserThreeMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton3").value = "EC";
      }
      else if (doserThreeMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton3").value = "PH"
      }
      if (doserFourMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton4").value = "OFF";
      }
      else if (doserFourMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton4").value = "EC";
      }
      else if (doserFourMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton4").value = "PH"
      }
      document.getElementById("d1mls").innerHTML = refillDoserOneMills;
      document.getElementById("d2mls").innerHTML = refillDoserTwoMills;
      document.getElementById("d3mls").innerHTML = refillDoserThreeMills;
      document.getElementById("d4mls").innerHTML = refillDoserFourMills;
    }
    else if (waterPageDoserScrollPosition == 1) {
      document.getElementById("waterDoser1").innerHTML = "2";
      document.getElementById("waterDoser2").innerHTML = "3";
      document.getElementById("waterDoser3").innerHTML = "4";
      document.getElementById("waterDoser4").innerHTML = "5";
      if (doserTwoMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton1").value = "OFF";
      }
      else if (doserTwoMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton1").value = "EC";
      }
      else if (doserTwoMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton1").value = "PH";
      }
      if (doserThreeMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton2").value = "OFF";
      }
      else if (doserThreeMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton2").value = "EC";
      }
      else if (doserThreeMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton2").value = "PH"
      }
      if (doserFourMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton3").value = "OFF";
      }
      else if (doserFourMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton3").value = "EC";
      }
      else if (doserFourMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton3").value = "PH"
      }
      if (doserFiveMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton4").value = "OFF";
      }
      else if (doserFiveMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton4").value = "EC";
      }
      else if (doserFiveMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton4").value = "PH"
      }
      document.getElementById("d1mls").innerHTML = refillDoserTwoMills;
      document.getElementById("d2mls").innerHTML = refillDoserThreeMills;
      document.getElementById("d3mls").innerHTML = refillDoserFourMills;
      document.getElementById("d4mls").innerHTML = refillDoserFiveMills;
    }
    else if (waterPageDoserScrollPosition == 2) {
      document.getElementById("waterDoser1").innerHTML = "3";
      document.getElementById("waterDoser2").innerHTML = "4";
      document.getElementById("waterDoser3").innerHTML = "5";
      document.getElementById("waterDoser4").innerHTML = "6";
      if (doserThreeMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton1").value = "OFF";
      }
      else if (doserThreeMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton1").value = "EC";
      }
      else if (doserThreeMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton1").value = "PH";
      }
      if (doserFourMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton2").value = "OFF";
      }
      else if (doserFourMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton2").value = "EC";
      }
      else if (doserFourMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton2").value = "PH"
      }
      if (doserFiveMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton3").value = "OFF";
      }
      else if (doserFiveMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton3").value = "EC";
      }
      else if (doserFiveMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton3").value = "PH"
      }
      if (doserSixMode == doserModes.DOSER_OFF) {
        document.getElementById("waterEcButton4").value = "OFF";
      }
      else if (doserSixMode == doserModes.EC_DOSER) {
        document.getElementById("waterEcButton4").value = "EC";
      }
      else if (doserSixMode == doserModes.PH_DOSER) {
        document.getElementById("waterEcButton4").value = "PH"
      }
      document.getElementById("d1mls").innerHTML = refillDoserThreeMills;
      document.getElementById("d2mls").innerHTML = refillDoserFourMills;
      document.getElementById("d3mls").innerHTML = refillDoserFiveMills;
      document.getElementById("d4mls").innerHTML = refillDoserSixMills;
    }
  }
}

function doserPageSlider(inc) {
  var doserPages = numDosers - 4;
  if (inc < 0) {
    if (doserPageScrollPosition > 0) doserPageScrollPosition--;
    else doserPageScrollPosition = doserPages;
  }
  else if (inc > 0) {
    if (doserPageScrollPosition < doserPages) doserPageScrollPosition++;
    else doserPageScrollPosition = 0;
  }
  var currentSlideIcon = "doserSlideIcon" + (doserPageScrollPosition + 1);
  for (i = 1;i < 4;i++) {
    var slideIcon = "doserSlideIcon" + i;
    document.getElementById(slideIcon).style.opacity = "0";
  }
  if (doserPages > 0) {
    for (i = 1;i < doserPages + 2;i++) {
      var slideIcon = "doserSlideIcon" + i;
      document.getElementById(slideIcon).src = document.getElementById("slideIcon_blue").src;
      if (slideIcon == currentSlideIcon) {
          document.getElementById(slideIcon).style.filter = "grayscale(0%)";
          document.getElementById(slideIcon).style.opacity = "1";
      }
      else {
          document.getElementById(slideIcon).style.filter = "grayscale(100%)";
          document.getElementById(slideIcon).style.opacity = "1";
      }
    }
  }
  if (doserPageScrollPosition == 0) {
    document.getElementById("doser1").innerHTML = "1";
    document.getElementById("doser2").innerHTML = "2";
    document.getElementById("doser3").innerHTML = "3";
    document.getElementById("doser4").innerHTML = "4";
    if (doserOneMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton1").value = "OFF";
      document.getElementById("ecButton1").style.backgroundColor = "red";
    }
    else if (doserOneMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton1").value = "EC";
      document.getElementById("ecButton1").style.backgroundColor = "lime";
    }
    else if (doserOneMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton1").value = "PH";
      document.getElementById("ecButton1").style.backgroundColor = "lime";
    }
    if (doserTwoMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton2").value = "OFF";
      document.getElementById("ecButton2").style.backgroundColor = "red";
    }
    else if (doserTwoMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton2").value = "EC";
      document.getElementById("ecButton2").style.backgroundColor = "lime";
    }
    else if (doserTwoMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton2").value = "PH"
      document.getElementById("ecButton2").style.backgroundColor = "lime";
    }
    if (doserThreeMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton3").value = "OFF";
      document.getElementById("ecButton3").style.backgroundColor = "red";
    }
    else if (doserThreeMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton3").value = "EC";
      document.getElementById("ecButton3").style.backgroundColor = "lime";
    }
    else if (doserThreeMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton3").value = "PH"
      document.getElementById("ecButton3").style.backgroundColor = "lime";
    }
    if (doserFourMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton4").value = "OFF";
      document.getElementById("ecButton4").style.backgroundColor = "red";
    }
    else if (doserFourMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton4").value = "EC";
      document.getElementById("ecButton4").style.backgroundColor = "lime";
    }
    else if (doserFourMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton4").value = "PH"
      document.getElementById("ecButton4").style.backgroundColor = "lime";
    }
    document.getElementById("doserMls_1").innerHTML = doserOneMills;
    document.getElementById("doserMls_2").innerHTML = doserTwoMills;
    document.getElementById("doserMls_3").innerHTML = doserThreeMills;
    document.getElementById("doserMls_4").innerHTML = doserFourMills;
  }
  else if (doserPageScrollPosition == 1) {
    document.getElementById("doser1").innerHTML = "2";
    document.getElementById("doser2").innerHTML = "3";
    document.getElementById("doser3").innerHTML = "4";
    document.getElementById("doser4").innerHTML = "5";
    if (doserTwoMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton1").value = "OFF";
      document.getElementById("ecButton1").style.backgroundColor = "red";
    }
    else if (doserTwoMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton1").value = "EC";
      document.getElementById("ecButton1").style.backgroundColor = "lime";
    }
    else if (doserTwoMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton1").value = "PH";
      document.getElementById("ecButton1").style.backgroundColor = "lime";
    }
    if (doserThreeMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton2").value = "OFF";
      document.getElementById("ecButton2").style.backgroundColor = "red";
    }
    else if (doserThreeMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton2").value = "EC";
      document.getElementById("ecButton2").style.backgroundColor = "lime";
    }
    else if (doserThreeMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton2").value = "PH"
      document.getElementById("ecButton2").style.backgroundColor = "lime";
    }
    if (doserFourMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton3").value = "OFF";
      document.getElementById("ecButton3").style.backgroundColor = "red";
    }
    else if (doserFourMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton3").value = "EC";
      document.getElementById("ecButton3").style.backgroundColor = "lime";
    }
    else if (doserFourMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton3").value = "PH"
      document.getElementById("ecButton3").style.backgroundColor = "lime";
    }
    if (doserFiveMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton4").value = "OFF";
      document.getElementById("ecButton4").style.backgroundColor = "red";
    }
    else if (doserFiveMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton4").value = "EC";
      document.getElementById("ecButton4").style.backgroundColor = "lime";
    }
    else if (doserFiveMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton4").value = "PH"
      document.getElementById("ecButton4").style.backgroundColor = "lime";
    }
    document.getElementById("doserMls_1").innerHTML = doserTwoMills;
    document.getElementById("doserMls_2").innerHTML = doserThreeMills;
    document.getElementById("doserMls_3").innerHTML = doserFourMills;
    document.getElementById("doserMls_4").innerHTML = doserFiveMills;
  }
  else {
    document.getElementById("doser1").innerHTML = "3";
    document.getElementById("doser2").innerHTML = "4";
    document.getElementById("doser3").innerHTML = "5";
    document.getElementById("doser4").innerHTML = "6";
     if (doserThreeMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton1").value = "OFF";
      document.getElementById("ecButton1").style.backgroundColor = "red";
     }
    else if (doserThreeMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton1").value = "EC";
      document.getElementById("ecButton1").style.backgroundColor = "lime";
    }
    else if (doserThreeMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton1").value = "PH";
      document.getElementById("ecButton1").style.backgroundColor = "lime";
    }
    if (doserFourMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton2").value = "OFF";
      document.getElementById("ecButton2").style.backgroundColor = "red";
    }
    else if (doserFourMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton2").value = "EC";
      document.getElementById("ecButton2").style.backgroundColor = "lime";
    }
    else if (doserFourMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton2").value = "PH"
      document.getElementById("ecButton2").style.backgroundColor = "lime";
    }
    if (doserFiveMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton3").value = "OFF";
      document.getElementById("ecButton3").style.backgroundColor = "red";
    }
    else if (doserFiveMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton3").value = "EC";
      document.getElementById("ecButton3").style.backgroundColor = "lime";
    }
    else if (doserFiveMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton3").value = "PH"
      document.getElementById("ecButton3").style.backgroundColor = "lime";
    }
    if (doserSixMode == doserModes.DOSER_OFF) {
      document.getElementById("ecButton4").value = "OFF";
      document.getElementById("ecButton4").style.backgroundColor = "red";
    }
    else if (doserSixMode == doserModes.EC_DOSER) {
      document.getElementById("ecButton4").value = "EC";
      document.getElementById("ecButton4").style.backgroundColor = "lime";
    }
    else if (doserSixMode == doserModes.PH_DOSER) {
      document.getElementById("ecButton4").value = "PH"
      document.getElementById("ecButton4").style.backgroundColor = "lime";
    }
    document.getElementById("doserMls_1").innerHTML = doserThreeMills;
    document.getElementById("doserMls_2").innerHTML = doserFourMills;
    document.getElementById("doserMls_3").innerHTML = doserFiveMills;
    document.getElementById("doserMls_4").innerHTML = doserSixMills;
  }
}

function fanPageSlider(inc) {
  if (inc < 0) {
    if (fanPageScrollPosition > 0) fanPageScrollPosition--;
    else fanPageScrollPosition = 4;
  } 
  else if (inc > 0) {
    if (fanPageScrollPosition < 4) fanPageScrollPosition++;
    else fanPageScrollPosition = 0;
  }
  var currentSlideIcon = "fanSlideIcon" + (fanPageScrollPosition + 1);
  for (i = 1;i < 6;i++) {
    var slideIcon = "fanSlideIcon" + i;
    var newPage = "fanPage" + i;
    document.getElementById(slideIcon).src = document.getElementById("slideIcon_blue").src;
    if (slideIcon == currentSlideIcon) {
      document.getElementById(slideIcon).style.filter = "grayscale(0%)";
      document.getElementById(newPage).style.display = "block";
    } 
    else {
      document.getElementById(slideIcon).style.filter = "grayscale(100%)";
      document.getElementById(newPage).style.display = "none";
    }
  }
}

function warningPageSlider(inc) {
  if (inc < 0) {
    if (warningsPageScrollPosition > 0) warningsPageScrollPosition--;
    else warningsPageScrollPosition = 7;
  } else if (inc > 0) {
    if (warningsPageScrollPosition < 7) warningsPageScrollPosition++;
    else warningsPageScrollPosition = 0;
  }
  var currentSlideIcon = "warningSlideIcon" + (warningsPageScrollPosition + 1);
  for (i = 1;i < 9;i++) {
    var slideIcon = "warningSlideIcon" + i;
    document.getElementById(slideIcon).src = document.getElementById("slideIcon_blue").src;
    if (slideIcon == currentSlideIcon) 
      document.getElementById(slideIcon).style.filter = "grayscale(0%)";
    else
      document.getElementById(slideIcon).style.filter = "grayscale(100%)";
  }

  if (warningsPageScrollPosition == 0) {
    document.getElementById("warningTitle").innerHTML = "EC warning";
    document.getElementById("sensorWarning").innerHTML = parseFloat(ecSensor).toFixed(2);
    document.getElementById("warningMargin").innerHTML = parseFloat(ecErrorMargin).toFixed(2);
    document.getElementById("warningMarginIcon").innerHTML = "";
    document.getElementById("warningSensorIcon").innerHTML = "";
    document.getElementById("sensorWarningDiv").style.alignItems = "initial";
    document.getElementById("warningMarginDiv").style.alignItems = "initial";
    setWarningTextColor("sensorWarning", ecErrorState);
  }
  else if (warningsPageScrollPosition == 1) {
    document.getElementById("warningTitle").innerHTML = "TDS warning";
    document.getElementById("sensorWarning").innerHTML = tdsSensor;
    document.getElementById("warningMargin").innerHTML = tdsErrorMargin;
    document.getElementById("warningMarginIcon").innerHTML = "";
    document.getElementById("warningSensorIcon").innerHTML = "";
    document.getElementById("sensorWarningDiv").style.alignItems = "initial";
    document.getElementById("warningMarginDiv").style.alignItems = "initial";
    setWarningTextColor("sensorWarning", ppmErrorState);
  }
  else if (warningsPageScrollPosition == 2) {
    document.getElementById("warningTitle").innerHTML = "PH warning";
    document.getElementById("sensorWarning").innerHTML = parseFloat(phSensor).toFixed(2);
    document.getElementById("warningMargin").innerHTML = parseFloat(phErrorMargin).toFixed(2);
    document.getElementById("warningMarginIcon").innerHTML = "";
    document.getElementById("warningSensorIcon").innerHTML = "";
    document.getElementById("sensorWarningDiv").style.alignItems = "initial";
    document.getElementById("warningMarginDiv").style.alignItems = "initial";
    setWarningTextColor("sensorWarning", phErrorState);
  }
  else if (warningsPageScrollPosition == 3) {
    document.getElementById("warningTitle").innerHTML = "Co2 warning";
    document.getElementById("sensorWarning").innerHTML = co2Sensor;
    document.getElementById("warningMargin").innerHTML = co2ErrorMargin;
    document.getElementById("warningMarginIcon").innerHTML = "ppm";
    document.getElementById("warningSensorIcon").innerHTML = "ppm";
    document.getElementById("sensorWarningDiv").style.alignItems = "baseline";
    document.getElementById("warningMarginDiv").style.alignItems = "baseline";
    setWarningTextColor("sensorWarning", co2ErrorState);
  }
  else if (warningsPageScrollPosition == 4) {
    document.getElementById("warningTitle").innerHTML = "Water level warning";
    if (convertToInchs) {
      document.getElementById("warningMarginIcon").innerHTML = "\"";
      document.getElementById("warningSensorIcon").innerHTML = "\"";
      document.getElementById("sensorWarningDiv").style.alignItems = "initial";
      document.getElementById("warningMarginDiv").style.alignItems = "initial";
    } 
    else {
      document.getElementById("warningMarginIcon").innerHTML = "cm";
      document.getElementById("warningSensorIcon").innerHTML = "cm";
      document.getElementById("sensorWarningDiv").style.alignItems = "baseline";
      document.getElementById("warningMarginDiv").style.alignItems = "baseline";
    }
    document.getElementById("sensorWarning").innerHTML = parseFloat(waterLevelSensor).toFixed(1);
    document.getElementById("warningMargin").innerHTML = parseFloat(waterHeightErrorMargin).toFixed(1);
    setWarningTextColor("sensorWarning", waterLevelErrorState);
  }
  else if (warningsPageScrollPosition == 5) {
    document.getElementById("warningTitle").innerHTML = "Water temp warning";
    document.getElementById("sensorWarningDiv").style.alignItems = "initial";
    document.getElementById("warningMarginDiv").style.alignItems = "initial";
    if (convertToF) {
      document.getElementById("warningMarginIcon").innerHTML = "f";
      document.getElementById("warningSensorIcon").innerHTML = "f";
    } 
    else {
      document.getElementById("warningMarginIcon").innerHTML = "c";
      document.getElementById("warningSensorIcon").innerHTML = "c";
    }
    document.getElementById("sensorWarning").innerHTML = parseFloat(waterTemperatureSensor).toFixed(1);
    document.getElementById("warningMargin").innerHTML = parseFloat(waterTempErrorMargin).toFixed(1);
    setWarningTextColor("sensorWarning", waterTempErrorState);
  }
  else if (warningsPageScrollPosition == 6) {
    document.getElementById("warningTitle").innerHTML = "Air temp warning";
    document.getElementById("sensorWarningDiv").style.alignItems = "initial";
    document.getElementById("warningMarginDiv").style.alignItems = "initial";
    if (convertToF) {
      document.getElementById("warningMarginIcon").innerHTML = "f";
      document.getElementById("warningSensorIcon").innerHTML = "f";
    } 
    else {
      document.getElementById("warningMarginIcon").innerHTML = "c";
      document.getElementById("warningSensorIcon").innerHTML = "c"; 
    }
    document.getElementById("sensorWarning").innerHTML = parseFloat(airTemperatureSensor).toFixed(1);
    document.getElementById("warningMargin").innerHTML = parseFloat(airTempErrorMargin).toFixed(1);
    setWarningTextColor("sensorWarning", airTempErrorState);
  }
  else if (warningsPageScrollPosition == 7) {
    document.getElementById("sensorWarningDiv").style.alignItems = "baseline";
    document.getElementById("warningMarginDiv").style.alignItems = "baseline";
    document.getElementById("warningTitle").innerHTML = "Humidity warning";
    document.getElementById("warningMarginIcon").innerHTML = "%";
    document.getElementById("warningSensorIcon").innerHTML = "%";
    document.getElementById("sensorWarning").innerHTML = parseFloat(airHumiditySensor).toFixed(1);
    document.getElementById("warningMargin").innerHTML = parseFloat(humidityErrorMargin).toFixed(1);
    setWarningTextColor("sensorWarning", humidityErrorState);
  }
}

function saveProfileName(profile) {
  if (profile == 0) {
    profileOneName = document.getElementById("profileInputBox_1").value;
    setVal("profileOneName", profileOneName);
  }
  else if (profile == 1) {
    profileTwoName = document.getElementById("profileInputBox_2").value;
    setVal("profileTwoName", profileTwoName);
  }
  else if (profile == 2) {
    profileThreeName = document.getElementById("profileInputBox_3").value;
    setVal("profileThreeName", profileThreeName);
  }
  else if (profile == 3) {
    profileFourName = document.getElementById("profileInputBox_4").value;
    setVal("profileFourName", profileFourName);
  }
  else if (profile == 4) {
    profileFiveName = document.getElementById("profileInputBox_5").value;
    setVal("profileFiveName", profileFiveName);
  }
}

function loadProfile(profile) {
  loadProfileNum = profile;
  for (var i = 0; i < 5; i++) {
    document.getElementById('profileInputBox_' + (i + 1)).style.border = (loadProfileNum == i ? "medium solid #0000FF" : "none");
  }
  setVal("loadData", loadProfileNum);
}

function saveProfile(profile) {
  saveProfileNum = profile;
  for (var i = 0; i < 5; i++) {
    document.getElementById('profileInputBox_' + (i + 1)).style.border = (saveProfileNum == i ? "medium solid #0000FF" : "none");
  }
  setVal("saveData", saveProfileNum);
}

function updateCalanderDays(day) {
  parseInt(autoFillDays[day-1]) == 1 ? autoFillDays[day-1] = 0 : autoFillDays[day-1] = 1;
  var calanderDay = "calanderDay" + day;
  if (autoFillDays[day-1] == 1) 
    document.getElementById(calanderDay).style.backgroundColor = "#0066ff";
  else
    document.getElementById(calanderDay).style.backgroundColor = "white";
  var str = "";
  for (var i = 0; i < 31; i++) {
    parseInt(autoFillDays[i]) == 1 ? str += "1," :str += "0,";
  }
  setVal("autoFillDays", str);
}

function adjustRefillTankTime(inc) {
  var refillTime = adjustRefillTime(inc, autoFillMinute, autoFillHour);
  document.getElementById("refillTankTime").innerHTML = refillTime;
}

function adjustDisableRefillAndDrain(inc) {
  disableDrainAndRefill = inc;
  setToggleButton(disableDrainAndRefill,"dRefillDrain");
  setVal("disableRefillDrain",disableDrainAndRefill);
}

function adjustWaterDoserMls(inc, ID) {
  if (waterPageDoserScrollPosition == 0) {
    if (ID == "d1mls") {
      refillDoserOneMills = adjustValue(inc, refillDoserOneMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserOneMills;
    }
    else if (ID == "d2mls") {
      refillDoserTwoMills = adjustValue(inc, refillDoserTwoMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserTwoMills;
    }
    else if (ID == "d3mls") {
      refillDoserThreeMills = adjustValue(inc, refillDoserThreeMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserThreeMills;
    }
    else if (ID == "d4mls") {
      refillDoserFourMills = adjustValue(inc, refillDoserFourMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserFourMills;
    }
  }
  else if (waterPageDoserScrollPosition == 1) {
    if (ID == "d1mls") {
      refillDoserTwoMills = adjustValue(inc, refillDoserTwoMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserTwoMills;
    }
    else if (ID == "d2mls") {
      refillDoserThreeMills = adjustValue(inc, refillDoserThreeMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserThreeMills;
    }
    else if (ID == "d3mls") {
      refillDoserFourMills = adjustValue(inc, refillDoserFourMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserFourMills;
    }
    else if (ID == "d4mls") {
      refillDoserFiveMills = adjustValue(inc, refillDoserFiveMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserFiveMills;
    }
  }
  else if (waterPageDoserScrollPosition == 2) {
    if (ID == "d1mls") {
      refillDoserThreeMills = adjustValue(inc, refillDoserThreeMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserThreeMills;
    }
    else if (ID == "d2mls") {
      refillDoserFourMills = adjustValue(inc, refillDoserFourMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserFourMills;
    }
    else if (ID == "d3mls") {
      refillDoserFiveMills = adjustValue(inc, refillDoserFiveMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserFiveMills;
    }
    else if (ID == "d4mls") {
      refillDoserSixMills = adjustValue(inc, refillDoserSixMills, 0, 999, 0, ID);
      document.getElementById(ID).innerHTML = refillDoserSixMills;
    }
  }
}

function adjustnumDosers(inc) {
  if (inc != null) numDosers = adjustValue(inc, numDosers, 4, 6, 0, "numDosers");
  document.getElementById("numDosers").innerHTML = numDosers;
}

function adjustdosingInt(inc) {
  if (inc != null) dosingInt = adjustValue(inc, dosingInt, 1, 99, 0, "dosingInt");
  document.getElementById("dosingInt").innerHTML = dosingInt;
}

function adjustTempInF(inc) {
  convertToF = inc;
  setToggleButton(convertToF,"tempInF");      
  updateCfIcons();
  setVal("convertToF", convertToF);
}

function adjustHeightInInchs(inc) {
  convertToInchs = inc;
  setToggleButton(convertToInchs,"heightInInchs");        
  updateCmIcons();
  setVal("convertToInchs", convertToInchs);
}

function adjustEcToTds(inc) {
  convertToTds = inc;
  setToggleButton(convertToTds,"ecToTds");
  updateEcIcons();
  setVal("convertToTds", convertToTds);
}

function adjustdisableCo2Warn(inc) {
  disableCo2Warn = inc;
  setToggleButton(disableCo2Warn,"disableCo2Warn");
  setVal("disableCo2Warn", disableCo2Warn);
}

function adjustdisableEcWarn(inc) {
  disableEcWarn = inc;
  setToggleButton(disableEcWarn,"disableEcWarn");
  setVal("disableEcWarn", disableEcWarn);
}

function adjustdisablePpmWarn(inc) {
  disablePpmWarn = inc;
  setToggleButton(disablePpmWarn,"disablePpmWarn");
  setVal("disablePpmWarn", disablePpmWarn);
}

function adjustdisablePhWarn(inc) {
  disablePhWarn = inc;
  setToggleButton(disablePhWarn,"disablePhWarn");
  setVal("disablePhWarn", disablePhWarn);
}

function adjustdisableWaterTempWarn(inc) {
  disableWaterTempWarn = inc;
  setToggleButton(disableWaterTempWarn,"disableWaterTempWarn");
  setVal("disableWaterTempWarn", disableWaterTempWarn);
}

function adjustdisableWaterHeightWarn(inc) {
  disableWaterHeightWarn = inc;
  setToggleButton(disableWaterHeightWarn,"disableWaterHeightWarn");
  setVal("disableWaterHeightWarn", disableWaterHeightWarn);
}

function adjustdisableAirTempWarn(inc) {
  disableAirTempWarn = inc;
  setToggleButton(disableAirTempWarn,"disableAirTempWarn");
  setVal("disableAirTempWarn", disableAirTempWarn);
}

function adjustdisableHumidityWarn(inc) {
  disableHumidityWarn = inc;
  setToggleButton(disableHumidityWarn,"disableHumidityWarn");
  setVal("disableHumidityWarn", disableHumidityWarn);
}

function adjustMinWaterLvl(inc) {
  targetMinWaterHeight = adjustValue(inc, targetMinWaterHeight, 0, targetMaxWaterHeight, 1, "targetMinWaterLvl");
  document.getElementById("targetMinWaterLvl").innerHTML = parseFloat(targetMinWaterHeight).toFixed(1);
}

function adjustMaxWaterLvl(inc) {
  targetMaxWaterHeight = adjustValue(inc, targetMaxWaterHeight, targetMinWaterHeight, 999, 1, "targetMaxWaterLvl");
  document.getElementById("targetMaxWaterLvl").innerHTML = parseFloat(targetMaxWaterHeight).toFixed(1);
}

function adjustWaterLvlOffset(inc) {
  waterHeightOffset = adjustValue(inc, waterHeightOffset, 0, 10, 1, "targetWaterLvlOffset");
  document.getElementById("targetWaterLvlOffset").innerHTML = parseFloat(waterHeightOffset).toFixed(1);
}

function adjustMinWaterTemp(inc) {
  targetMinWaterTemp = adjustValue(inc, targetMinWaterTemp, 0, targetMaxWaterTemp, 1, "targetMinWaterTemp");
  document.getElementById("targetMinWaterTemp").innerHTML = parseFloat(targetMinWaterTemp).toFixed(1);
}

function adjustMaxWaterTemp(inc) {
  targetMaxWaterTemp = adjustValue(inc, targetMaxWaterTemp, targetMinWaterTemp, 999, 1, "targetMaxWaterTemp");
  document.getElementById("targetMaxWaterTemp").innerHTML = parseFloat(targetMaxWaterTemp).toFixed(1);
}

function adjustWaterTempOffset(inc) {
  waterTempOffset = adjustValue(inc, waterTempOffset, 0, 10, 1, "targetWaterTempOffset");
  document.getElementById("targetWaterTempOffset").innerHTML = parseFloat(waterTempOffset).toFixed(1);
}

function adjustMinEc(inc) {
  if (inc != null) {
    if(convertToTds)
      targetMinTds = adjustValue(100*inc, targetMinTds, 1, targetMaxTds, 2, "targetMinTds");
    else
      targetMinEc = adjustValue(inc, targetMinEc, 0.01, targetMaxEc, 2, "targetMinEc");
  }
  if(convertToTds)
    document.getElementById("targetMinEc").innerHTML = parseFloat(targetMinTds).toFixed(0);
  else
    document.getElementById("targetMinEc").innerHTML = parseFloat(targetMinEc).toFixed(2);
}

function adjustMaxEc(inc) {
 console.log(100*inc);
  if (inc != null) {
    if(convertToTds)
      targetMaxTds = adjustValue(100*inc, targetMaxTds, targetMinTds, 999, 2, "targetMaxTds");
    else
      targetMaxEc = adjustValue(inc, targetMaxEc, targetMinEc, 6, 2, "targetMaxEc");
  }
  if(convertToTds)
    document.getElementById("targetMaxEc").innerHTML = parseFloat(targetMaxTds).toFixed(0);
  else
    document.getElementById("targetMaxEc").innerHTML = parseFloat(targetMaxEc).toFixed(2);
}

function adjustEcOffset(inc) {
  if (inc != null) {
    if(convertToTds)
      tdsOffset = adjustValue(100*inc, tdsOffset, 0, 1, 10, "tdsOffset");
    else
      ecOffset = adjustValue(inc, ecOffset, 0, 1, 2, "ecOffset");
  }
  if(convertToTds)
    document.getElementById("targetEcOffset").innerHTML = parseFloat(tdsOffset).toFixed(0);
  else
    document.getElementById("targetEcOffset").innerHTML = parseFloat(ecOffset).toFixed(2);
}

function adjustMinPh(inc) {
  if (inc != null) targetMinPh = adjustValue(inc, targetMinPh, 0.01, targetMaxPh, 2, "targetMinPh");
  document.getElementById("targetMinPh").innerHTML = parseFloat(targetMinPh).toFixed(2);
}

function adjustMaxPh(inc) {
  if (inc != null) targetMaxPh = adjustValue(inc, targetMaxPh, targetMinPh, 14, 2, "targetMaxPh");
  document.getElementById("targetMaxPh").innerHTML = parseFloat(targetMaxPh).toFixed(2);
}

function adjustPhOffset(inc) {
  if (inc != null) phOffset = adjustValue(inc, phOffset, 0, 1, 2, "phOffset");
  document.getElementById("targetPhOffset").innerHTML = parseFloat(phOffset).toFixed(2);
}

function adjustTargetCo2(inc) {
  if (inc != null) targetCo2 = adjustValue(inc, targetCo2, 400, 9999, 0, "targetCo2");
  document.getElementById("targetCo2").innerHTML = targetCo2;
  document.getElementById("targetCo2Symbol").innerHTML = "ppm";
}

function adjustTargetCo2Offset(inc) {
  if (inc != null) co2Offset = adjustValue(inc, co2Offset, 0, 100, 0, "co2Offset");
  document.getElementById("targetCo2Offset").innerHTML = co2Offset;
}

function adjustCo2RoomLength(inc) {
  if (inc != null) roomLength = adjustValue(inc, roomLength, 10, 9999, 0, "roomLength");
  document.getElementById("co2RoomLength").innerHTML = roomLength;
}

function adjustCo2RoomWidth(inc) {
  if (inc != null) roomWidth = adjustValue(inc, roomWidth, 10, 9999, 0, "roomWidth");
  document.getElementById("co2RoomWidth").innerHTML = roomWidth;
}

function adjustCo2RoomHeight(inc) {
  if (inc != null) roomHeight = adjustValue(inc, roomHeight, 10, 9999, 0, "roomHeight");
  document.getElementById("co2RoomHeight").innerHTML = roomHeight;
}

function adjustCo2FlowRate(inc) {
  if (inc != null) co2Flowrate = adjustValue(inc, co2Flowrate, 0.1, 99, 1, "co2Flowrate");
  document.getElementById("co2FlowRate").innerHTML = parseFloat(co2Flowrate).toFixed(1);
}

function adjustCo2Interval(inc) {
  var co2Time = adjustCo2Time(inc, co2CheckTimeMinute, co2CheckTimeHour);
  document.getElementById("co2Interval").innerHTML = co2Time;
  if (inc != null) {
    setVal("co2CheckTimeMin", co2CheckTimeMinute);
    setVal("co2CheckTimeHour", co2CheckTimeHour);
  }
}

function adjustCo2FanDisableTime(inc) {
  if (inc != null) disableFansTimer = adjustValue(inc, disableFansTimer, 0, 999, 0, "disableFansTimer");
  document.getElementById("co2DisableFanTimer").innerHTML = disableFansTimer;
}

function adjustLightOn(inc) {
  var onTime = adjustLightOnTime(inc, onTimeMin, onTimeHour);
  document.getElementById("lightOnTime").innerHTML = onTime;
  if (inc != null) {
    setVal("onTimeMin", onTimeMin);
    setVal("onTimeHour", onTimeHour);
  }
}

function adjustLightOff(inc) {
  var offTime = adjustLightOffTime(inc, offTimeMin, offTimeHour);
  document.getElementById("lightOffTime").innerHTML = offTime;
  if (inc != null) {
    setVal("offTimeMin", offTimeMin);
    setVal("offTimeHour", offTimeHour);
  }
}

function adjustFanOneMinSpeed(inc) {
  if (inc != null) fanOneMinSpeed = adjustValue(inc, fanOneMinSpeed, 0, fanOneMaxSpeed, 0, "fanOneMinSpeed");
  document.getElementById("fanOneMinSpeed").innerHTML = fanOneMinSpeed;
}

function adjustFanOneMaxSpeed(inc) {
  if (inc != null) fanOneMaxSpeed = adjustValue(inc, fanOneMaxSpeed, fanOneMinSpeed, 100, 0, "fanOneMaxSpeed");
  document.getElementById("fanOneMaxSpeed").innerHTML = fanOneMaxSpeed;
}

function adjustFanTwoMinSpeed(inc) {
  if (inc != null) fanTwoMinSpeed = adjustValue(inc, fanTwoMinSpeed, 0, fanTwoMaxSpeed, 0, "fanTwoMinSpeed");
  document.getElementById("fanTwoMinSpeed").innerHTML = fanTwoMinSpeed;
}

function adjustFanTwoMaxSpeed(inc) {
  if (inc != null) fanTwoMaxSpeed = adjustValue(inc, fanTwoMaxSpeed, fanTwoMinSpeed, 100, 0, "fanTwoMaxSpeed");
  document.getElementById("fanTwoMaxSpeed").innerHTML = fanTwoMaxSpeed;
}

function adjustMinAirTemp(inc) {
  if (inc != null) targetMinAirTemp = adjustValue(inc, targetMinAirTemp, 0, targetMaxAirTemp, 1, "targetMinAirTemp");
  document.getElementById("minAirTemp").innerHTML = parseFloat(targetMinAirTemp).toFixed(1);
}

function adjustMaxAirTemp(inc) {
  if (inc != null) targetMaxAirTemp = adjustValue(inc, targetMaxAirTemp, targetMinAirTemp, 999, 1, "targetMaxAirTemp");
  document.getElementById("maxAirTemp").innerHTML = parseFloat(targetMaxAirTemp).toFixed(1);
}

function adjustAirTempOffset(inc) {
  if (inc != null) airTempOffset = adjustValue(inc, airTempOffset, 0, 10, 1, "airTempOffset");
  document.getElementById("airTempOffset").innerHTML = parseFloat(airTempOffset).toFixed(1);
}

function adjustMinHumidity(inc) {
  if (inc != null) targetMinHumidity = adjustValue(inc, targetMinHumidity, 0, targetMaxHumidity, 1, "targetMinHumidity");
  document.getElementById("minHumidity").innerHTML = parseFloat(targetMinHumidity).toFixed(1);
}

function adjustMaxHumidity(inc) {
  if (inc != null) targetMaxHumidity = adjustValue(inc, targetMaxHumidity, targetMinHumidity, 999, 1, "targetMaxHumidity");
  document.getElementById("maxHumidity").innerHTML = parseFloat(targetMaxHumidity).toFixed(1);
}

function adjustHumidityOffset(inc) {
  if (inc != null) humidityOffset = adjustValue(inc, humidityOffset, 0, 10, 1, "humidityOffset");
  document.getElementById("humidityOffset").innerHTML = parseFloat(humidityOffset).toFixed(1);
}

function adjustControlTemp(inc) {
  fansControlTemp = inc;
  setToggleButton(fansControlTemp,"controlTemp");
  setVal("fansControlTemp", fansControlTemp);
}

function adjustControlHumidity(inc) {
  fansControlHum = inc;
  setToggleButton(fansControlHum,"controlHum");
  setVal("fansControlHum", fansControlHum);
}

function adjustFanOneFixedSpeed(inc) {
  fanOneFixedSpeed = inc;
  setToggleButton(fanOneFixedSpeed,"fanOneFixedSpeed");
  setVal("fanOneFixedSpeed", fanOneFixedSpeed);
}

function adjustFanTwoFixedSpeed(inc) {
  fanTwoFixedSpeed = inc;
  setToggleButton(fanTwoFixedSpeed,"fanTwoFixedSpeed");
  setVal("fanTwoFixedSpeed", fanTwoFixedSpeed);
}

function adjustWarningMargin(inc) {
  if (warningsPageScrollPosition == 0) {
    ecErrorMargin = adjustValue(0.01 * inc, ecErrorMargin, 0, 1, 2, "ecErrorMargin");
    document.getElementById("warningMargin").innerHTML = parseFloat(ecErrorMargin).toFixed(2);
  }
  else if (warningsPageScrollPosition == 1) {
    tdsErrorMargin = adjustValue(inc, tdsErrorMargin, 0, 50, 0, "tdsErrorMargin");
    document.getElementById("warningMargin").innerHTML = tdsErrorMargin;
  }
  else if (warningsPageScrollPosition == 2) {
    phErrorMargin = adjustValue(0.01 * inc, phErrorMargin, 0, 1, 2, "phErrorMargin");
    document.getElementById("warningMargin").innerHTML = parseFloat(phErrorMargin).toFixed(2);
  }
  else if (warningsPageScrollPosition == 3) {
    co2ErrorMargin = adjustValue(inc, co2ErrorMargin, 0, 50, 0, "co2ErrorMargin");
    document.getElementById("warningMargin").innerHTML = co2ErrorMargin;
  }
  else if (warningsPageScrollPosition == 4) {
    waterHeightErrorMargin = adjustValue(0.1 * inc, waterHeightErrorMargin, 0, 10, 1, "waterHeightErrorMargin");
    document.getElementById("warningMargin").innerHTML = parseFloat(waterHeightErrorMargin).toFixed(1);
  }
  else if (warningsPageScrollPosition == 5) {
    waterTempErrorMargin = adjustValue(0.1 * inc, waterTempErrorMargin, 0, 10, 1, "waterTempErrorMargin");
    document.getElementById("warningMargin").innerHTML = parseFloat(waterTempErrorMargin).toFixed(1);
  }
  else if (warningsPageScrollPosition == 6) {
    airTempErrorMargin = adjustValue(0.1 * inc, airTempErrorMargin, 0, 10, 1, "airTempErrorMargin");
    document.getElementById("warningMargin").innerHTML = parseFloat(airTempErrorMargin).toFixed(1);
  }
  else if (warningsPageScrollPosition == 7) {
    humidityErrorMargin = adjustValue(0.1 * inc, humidityErrorMargin, 0, 10, 1, "humidityErrorMargin");
    document.getElementById("warningMargin").innerHTML = parseFloat(humidityErrorMargin).toFixed(1);
  }
}

function adjustDoserMls(inc, doser) {
  if (doserPageScrollPosition == 0) {
    if (doser == "doserMls_1") {
      doserOneMills = adjustValue(inc, doserOneMills, 0, 999, 0, "doserMls_1");
      document.getElementById(doser).innerHTML = doserOneMills;
    }
    else if (doser == "doserMls_2") {
      doserTwoMills = adjustValue(inc, doserTwoMills, 0, 999, 0, "doserMls_2");
      document.getElementById(doser).innerHTML = doserTwoMills;
    }
    else if (doser == "doserMls_3") {
      doserThreeMills = adjustValue(inc, doserThreeMills, 0, 999, 0, "doserMls_3");
      document.getElementById(doser).innerHTML = doserThreeMills;
    }
    else if (doser == "doserMls_4") {
      doserFourMills = adjustValue(inc, doserFourMills, 0, 999, 0, "doserMls_4");
      document.getElementById(doser).innerHTML = doserFourMills;
    }
  }
  else if (doserPageScrollPosition == 1) {
    if (doser == "doserMls_1") {
      doserTwoMills = adjustValue(inc, doserTwoMills, 0, 999, 0, "doserMls_2");
      document.getElementById(doser).innerHTML = doserTwoMills;
    }
    else if (doser == "doserMls_2") {
      doserThreeMills = adjustValue(inc, doserThreeMills, 0, 999, 0, "doserMls_3");
      document.getElementById(doser).innerHTML = doserThreeMills;
    }
    else if (doser == "doserMls_3") {
      doserFourMills = adjustValue(inc, doserFourMills, 0, 999, 0, "doserMls_4");
      document.getElementById(doser).innerHTML = doserFourMills;
    }
    else if (doser == "doserMls_4") {
      doserFiveMills = adjustValue(inc, doserFiveMills, 0, 999, 0, "doserMls_5");
      document.getElementById(doser).innerHTML = doserFiveMills;
    }
  }
  else {
    if (doser == "doserMls_1") {
      doserThreeMills = adjustValue(inc, doserThreeMills, 0, 999, 0, "doserMls_3");
      document.getElementById(doser).innerHTML = doserThreeMills;
    }
    else if (doser == "doserMls_2") {
      doserFourMills = adjustValue(inc, doserFourMills, 0, 999, 0, "doserMls_4");
      document.getElementById(doser).innerHTML = doserFourMills;
    }
    else if (doser == "doserMls_3") {
      doserFiveMills = adjustValue(inc, doserFiveMills, 0, 999, 0, "doserMls_5");
      document.getElementById(doser).innerHTML = doserFiveMills;
    }
    else if (doser == "doserMls_4") {
      doserSixMills = adjustValue(inc, doserSixMills, 0, 999, 0, "doserMls_6");
      document.getElementById(doser).innerHTML = doserSixMills;
    }
  }
}

function changeDoserButton(doser) {
  if (doserPageScrollPosition == 0) {
    if (doser == "ecButton1") {
      if (doserOneMode < 2)
        doserOneMode++;
      else
        doserOneMode = 0;
      if (doserOneMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton1").value = "OFF";
        document.getElementById("ecButton1").style.backgroundColor = "red";
      }
      else if (doserOneMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton1").value = "EC";
        document.getElementById("ecButton1").style.backgroundColor = "lime";
      }
      else if (doserOneMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton1").value = "PH";
        document.getElementById("ecButton1").style.backgroundColor = "lime";
      }
      setVal("doserOneMode", doserOneMode);
    }
    else if (doser == "ecButton2") {
      if (doserTwoMode < 2)
        doserTwoMode++;
      else
        doserTwoMode = 0;
      if (doserTwoMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton2").value = "OFF";
        document.getElementById("ecButton2").style.backgroundColor = "red";
      }
      else if (doserTwoMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton2").value = "EC";
        document.getElementById("ecButton2").style.backgroundColor = "lime";
      }
      else if (doserTwoMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton2").value = "PH"
        document.getElementById("ecButton2").style.backgroundColor = "lime";
      }
      setVal("doserTwoMode", doserTwoMode);
    }
    else if (doser == "ecButton3") {
      if (doserThreeMode < 2)
        doserThreeMode++;
      else
        doserThreeMode = 0;
      if (doserThreeMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton3").value = "OFF";
        document.getElementById("ecButton3").style.backgroundColor = "red";
      }
      else if (doserThreeMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton3").value = "EC";
        document.getElementById("ecButton3").style.backgroundColor = "lime";
      }
      else if (doserThreeMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton3").value = "PH"
        document.getElementById("ecButton3").style.backgroundColor = "lime";
      }
      setVal("doserThreeMode", doserThreeMode);
    }
    else if (doser == "ecButton4") {
      if (doserFourMode < 2)
        doserFourMode++;
      else
        doserFourMode = 0;
      if (doserFourMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton4").value = "OFF";
        document.getElementById("ecButton4").style.backgroundColor = "red";
      }
      else if (doserFourMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton4").value = "EC";
        document.getElementById("ecButton4").style.backgroundColor = "lime";
      }
      else if (doserFourMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton4").value = "PH"
        document.getElementById("ecButton4").style.backgroundColor = "lime";
      }
      setVal("doserFourMode", doserFourMode);
    }
  }
  else if (doserPageScrollPosition == 1) {
    if (doser == "ecButton1") {
      if (doserTwoMode < 2)
        doserTwoMode++;
      else
        doserTwoMode = 0;
      if (doserTwoMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton1").value = "OFF";
        document.getElementById("ecButton1").style.backgroundColor = "red";
      }
      else if (doserTwoMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton1").value = "EC";
        document.getElementById("ecButton1").style.backgroundColor = "lime";
      }
      else if (doserTwoMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton1").value = "PH";
        document.getElementById("ecButton1").style.backgroundColor = "lime";
      }
      setVal("doserTwoMode", doserTwoMode);
    }
    else if (doser == "ecButton2") {
      if (doserThreeMode < 2)
        doserThreeMode++;
      else
        doserThreeMode = 0;
      if (doserThreeMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton2").value = "OFF";
        document.getElementById("ecButton2").style.backgroundColor = "red";
      }
      else if (doserThreeMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton2").value = "EC";
        document.getElementById("ecButton2").style.backgroundColor = "lime";
      }
      else if (doserThreeMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton2").value = "PH"
        document.getElementById("ecButton2").style.backgroundColor = "lime";
      }
      setVal("doserThreeMode", doserThreeMode);
    }
    else if (doser == "ecButton3") {
      if (doserFourMode < 2)
        doserFourMode++;
      else
        doserFourMode = 0;
      if (doserFourMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton3").value = "OFF";
        document.getElementById("ecButton3").style.backgroundColor = "red";
      }
      else if (doserFourMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton3").value = "EC";
        document.getElementById("ecButton3").style.backgroundColor = "lime";
      }
      else if (doserFourMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton3").value = "PH"
        document.getElementById("ecButton3").style.backgroundColor = "lime";
      }
      setVal("doserFourMode", doserFourMode);
    }
    else if (doser == "ecButton4") {
      if (doserFiveMode < 2)
        doserFiveMode++;
      else
        doserFiveMode = 0;
      if (doserFiveMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton4").value = "OFF";
        document.getElementById("ecButton4").style.backgroundColor = "red";
      }
      else if (doserFiveMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton4").value = "EC";
        document.getElementById("ecButton4").style.backgroundColor = "lime";
      }
      else if (doserFiveMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton4").value = "PH"
        document.getElementById("ecButton4").style.backgroundColor = "lime";
      }
      setVal("doserFiveMode", doserFiveMode);
    }
  }
  else {
    if (doser == "ecButton1") {
      if (doserThreeMode < 2)
        doserThreeMode++;
      else
        doserThreeMode = 0;
      if (doserThreeMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton1").value = "OFF";
        document.getElementById("ecButton1").style.backgroundColor = "red";
       }
      else if (doserThreeMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton1").value = "EC";
        document.getElementById("ecButton1").style.backgroundColor = "lime";
      }
      else if (doserThreeMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton1").value = "PH";
        document.getElementById("ecButton1").style.backgroundColor = "lime";
      }
      setVal("doserThreeMode", doserThreeMode);
    }
    else if (doser == "ecButton2") {
      if (doserFourMode < 2)
        doserFourMode++;
      else
        doserFourMode = 0;
      if (doserFourMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton2").value = "OFF";
        document.getElementById("ecButton2").style.backgroundColor = "red";
      }
      else if (doserFourMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton2").value = "EC";
        document.getElementById("ecButton2").style.backgroundColor = "lime";
      }
      else if (doserFourMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton2").value = "PH"
        document.getElementById("ecButton2").style.backgroundColor = "lime";
      }
      setVal("doserFourMode", doserFourMode);
    }
    else if (doser == "ecButton3") {
      if (doserFiveMode < 2)
        doserFiveMode++;
      else
        doserFiveMode = 0;
      if (doserFiveMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton3").value = "OFF";
        document.getElementById("ecButton3").style.backgroundColor = "red";
      }
      else if (doserFiveMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton3").value = "EC";
        document.getElementById("ecButton3").style.backgroundColor = "lime";
      }
      else if (doserFiveMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton3").value = "PH"
        document.getElementById("ecButton3").style.backgroundColor = "lime";
      }
      setVal("doserFiveMode", doserFiveMode);
    }
    else if (doser == "ecButton4") {
      if (doserSixMode < 2)
        doserSixMode++;
      else
        doserSixMode = 0;
      if (doserSixMode == doserModes.DOSER_OFF) {
        document.getElementById("ecButton4").value = "OFF";
        document.getElementById("ecButton4").style.backgroundColor = "red";
      }
      else if (doserSixMode == doserModes.EC_DOSER) {
        document.getElementById("ecButton4").value = "EC";
        document.getElementById("ecButton4").style.backgroundColor = "lime";
      }
      else if (doserSixMode == doserModes.PH_DOSER) {
        document.getElementById("ecButton4").value = "PH"
        document.getElementById("ecButton4").style.backgroundColor = "lime";
      }
      setVal("doserSixMode", doserSixMode);
    }
  }
}

function adjustLightMode(mode) {
  lightMode = mode;
  if (lightMode == 0) {
    document.getElementById("lightOn").style.backgroundColor = "lightgrey";
    document.getElementById("lightAuto").style.backgroundColor = "lime";
    document.getElementById("lightOff").style.backgroundColor = "lightgrey";
  }
  else if (lightMode == 1) {
    document.getElementById("lightOn").style.backgroundColor = "lime";
    document.getElementById("lightAuto").style.backgroundColor = "lightgrey";
    document.getElementById("lightOff").style.backgroundColor = "lightgrey";
  }
  else if (lightMode == 2) {
    document.getElementById("lightOn").style.backgroundColor = "lightgrey";
    document.getElementById("lightAuto").style.backgroundColor = "lightgrey";
    document.getElementById("lightOff").style.backgroundColor = "red";
  }
  setVal("lightMode", lightMode);
}

function adjustValue(inc, v, min, max, pre, cmd) {
  inc= parseFloat(inc);
  v = parseFloat(v);
  min = parseFloat(min);
  max = parseFloat(max);
  if (inc < 0 && v > min + Math.abs(inc))
    v += parseFloat(inc);
  else if (inc < 0)
    v = min;
  if (inc > 0 && v < max - inc)
    v += parseFloat(inc);
  else if (inc > 0)
    v = max;
  setVal(cmd, parseFloat(v).toFixed(pre)); 
  console.log("adjustment = " + parseFloat(v).toFixed(pre));
  return v;
}

function adjustCo2Time(inc, minute, hour) {
  minute = parseInt(minute);
  hour = parseInt(hour);
  if (inc < 0) {
    if (minute >= 15) minute-=15;
    else {
      minute = 45;
      if (hour > 0) hour--;
      else hour = 23;
    }
  }
  else if (inc > 0) {
    if (minute <= 30) minute+=15;
    else {
      minute = 0;
      if (hour < 23) hour++;
      else hour = 0;
    }
  }
  co2CheckTimeMinute = minute;
  co2CheckTimeHour = hour;
  var strMin = minute;
  var strHour = hour;
  //var timeSymbol = hour < 10 ? "AM" :"PM";
  if (minute < 10) strMin = "0" + strMin;
  if (hour < 10) strHour = "0" + hour;
  return (strHour + ":" + strMin);// + timeSymbol
}

function gasTimeEnabled(inc) {
  manGasTimeEnabled = inc;
  setToggleButton(manGasTimeEnabled,"gasTimeEnabled");
  setVal("gasTimeEnabled", manGasTimeEnabled);
}

function adjustManualGasDurationTime(inc) {
  if (inc != null) manualGasDurationTime = adjustValue(inc, manualGasDurationTime, 0, 999, 0, "gasTime");
  document.getElementById("manualGasDurationTimer").innerHTML = manualGasDurationTime;
}

function disableCo2Control(inc) {
  co2ControlDisabled = inc;
  setToggleButton(co2ControlDisabled,"disableCo2Control");
  setVal("disableCo2Control", co2ControlDisabled);
}

function setToggleButton(val, ID) {
  //val = parseInt(val);
  //console.log("val:" + val + ", ID:" + ID);
  document.getElementById(ID+"Left").style.backgroundColor = (val ? "lime" : "lightgrey");
  document.getElementById(ID+"Right").style.backgroundColor = (val ? "lightgrey" : "red");
}

function adjustLightOnTime(inc, minute, hour) {
  minute = parseInt(minute);
  hour = parseInt(hour);
  if (inc < 0) {
    if (minute >= 15) minute-=15;
    else {
      minute = 45;
      if (hour > 0) hour--;
      else hour = 23;
    }
  }
  else if (inc > 0) {
    if (minute <= 30) minute+=15;
    else {
      minute = 0;
      if (hour < 23) hour++;
      else hour = 0;
    }
  }
  onTimeMin = minute;
  onTimeHour = hour;
  var strMin = minute;
  var strHour = hour;
  if (minute < 10) strMin = "0" + strMin;
  if (hour < 10) strHour = "0" + hour;
  return (strHour + ":" + strMin);
}

function adjustLightOffTime(inc, minute, hour) {
  minute = parseInt(minute);
  hour = parseInt(hour);
  if (inc < 0) {
    if (minute >= 15) minute-=15;
    else {
      minute = 45;
      if (hour > 0) hour--;
      else hour = 23;
    }
  }
  else if (inc > 0) {
    if (minute <= 30) minute+=15;
    else {
      minute = 0;
      if (hour < 23) hour++;
      else hour = 0;
    }
  }
  offTimeMin = minute;
  offTimeHour = hour;
  var strMin = minute;
  var strHour = hour;
  if (minute < 10) strMin = "0" + strMin;
  if (hour < 10) strHour = "0" + hour;
  return (strHour + ":" + strMin);
}

function adjustRefillTime(inc, minute, hour) {
  minute = parseInt(minute);
  hour = parseInt(hour);
  if (inc < 0) {
    if (minute >= 15) minute-=15;
    else {
      minute = 45;
      if (hour > 0) hour--;
      else hour = 23;
    }
  }
  else if (inc > 0) {
    if (minute <= 30) minute+=15;
    else {
      minute = 0;
      if (hour < 23) hour++;
      else hour = 0;
    }
  }
  autoFillMinute = minute;
  autoFillHour = hour;
  var strMin = minute;
  var strHour = hour;
  var timeSymbol = hour < 10 ? "AM" :"PM";
  if (minute < 10) strMin = "0" + strMin;
  if (hour < 10) strHour = "0" + hour;
  setVal("autoFillMinute", autoFillMinute);
  setVal("autoFillHour", autoFillHour);
  return ("Time " + strHour + ":" + strMin + timeSymbol);
}

function drawGraph(canvasID, a_xStartPos, a_yStartPos, a_graphHeight, a_graphWidth, a_numYLabels, a_displayQuadrant, a_data, a_arrayNumber, a_percision) {
  var yMax = 0;
  var maxGraphArrayValues = 12;
  var canvas = document.getElementById(canvasID);
  var ctx = canvas.getContext("2d");
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.font = (canvas.height/15)+"px Tahoma";
  ctx.fillStyle = "black";
  ctx.beginPath();
  // work out the greatest number to display on the y axis.
  for (var i = 0;i < a_arrayNumber;i++) {
    if (a_data[i] > yMax)
      yMax = a_data[i];
  }

  var graphXstartPosition = 0;
  var yinc = yMax / a_numYLabels;
  var YLablePosition = a_yStartPos + (canvasID == "graphCanvas2" ? (canvas.height/19) : 0);
  var tempYmax = yMax;
  var yLableGap = (a_graphHeight - 11) / a_numYLabels;

  for (var i = 0;i < a_numYLabels;i++) {
    ctx.fillText(parseFloat(tempYmax).toFixed(1), a_xStartPos, YLablePosition);
    var textMaxWidth = ctx.measureText(parseFloat(tempYmax).toFixed(1)).width + a_xStartPos;
  if (parseFloat(textMaxWidth) > parseFloat(graphXstartPosition))
      graphXstartPosition = textMaxWidth;
    YLablePosition += yLableGap;// 33
    tempYmax -= yinc;
  }

  ctx.fillText(0, a_xStartPos, YLablePosition);
  ctx.moveTo(graphXstartPosition + 2, a_yStartPos + a_graphHeight - 10);
  ctx.lineTo(graphXstartPosition + 2, a_yStartPos);
  ctx.stroke();

  // draw x line and numbers
  ctx.textBaseline = "top";
  var XSpacing = (a_graphWidth) / maxGraphArrayValues;
  var xLablePosition = graphXstartPosition + 5;
  for (var i = 1 ;i < a_arrayNumber + 1;i++) {
    ctx.fillText(i, xLablePosition, a_yStartPos + a_graphHeight - 10);
    xLablePosition += XSpacing;
  }

  ctx.moveTo(graphXstartPosition + 2, a_yStartPos + a_graphHeight - 11);
  ctx.lineTo(xLablePosition - (XSpacing / 1.5), a_yStartPos + a_graphHeight - 11);
  ctx.stroke();

  // draw line graph
  XSpacing += 1;
  var yGap = ((a_graphHeight - 12) / yMax);
  var xGap = graphXstartPosition + 6;
  var plotXstartPosition = 0, plotXendPosition = 0;
  if (a_arrayNumber >= 2) {
    for (var i = 0;i < a_arrayNumber - 1;i++) {
      plotYstartPosition = a_yStartPos + a_graphHeight - (yGap * a_data[i]);
      plotYendPosition = a_yStartPos + a_graphHeight - (yGap * a_data[i + 1]);
      ctx.moveTo(xGap, plotYstartPosition - 12);
      ctx.lineTo(xGap + XSpacing, plotYendPosition - 12);
      ctx.stroke();

      if (a_displayQuadrant){
        ctx.beginPath();
        ctx.arc(xGap, plotYstartPosition - 12, 2, 0, 2 * Math.PI, false);
        ctx.fillStyle = 'green';
        ctx.fill();
        ctx.stroke();
      }
      xGap += XSpacing;
    }
    // Very end quadrant to display
    if (a_displayQuadrant && a_arrayNumber >= 2){
      ctx.beginPath();
      ctx.arc(xGap, plotYendPosition - 12, 2, 0, 2 * Math.PI, false);
      ctx.fillStyle = 'green';
      ctx.fill();
      ctx.stroke();
    }
  }
}

setInterval(function() {
  getData();
  var currentMillis = new Date().getTime();
  if (currentMillis - connectionTime >= 3000) {
    document.getElementById("deviceTitleWarning").style.display = "inline";
    document.getElementById("deviceStatusTitle").style.color = "red";
    document.getElementById("deviceStatusTitle").innerHTML = "<b>Device disconnected</b>";
  }
  else {
    document.getElementById("deviceTitleWarning").style.display = "none";
    document.getElementById("deviceStatusTitle").style.color = "green";
    document.getElementById("deviceStatusTitle").innerHTML = "<b>Device connected</b>";
  }
},1000);   

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      console.log(this.responseText);
      var values = this.responseText.split(",");
      var cnt = 0;
      if (values.length == 286) {
        //Arrays
        for (var i = 0; i < 12; i++) 
          ecSenorArray[i] = values[cnt++];
        for (var i = 0; i < 12; i++) 
          tdsSenorArray[i] = values[cnt++];
        for (var i = 0; i < 12; i++) 
          phSenorArray[i] = values[cnt++];
        for (var i = 0; i < 12; i++) 
          co2SenorArray[i] = values[cnt++];  
        for (var i = 0; i < 12; i++) 
          waterTemperatureSenorArray[i] = values[cnt++];
        for (var i = 0; i < 12; i++) 
          waterLevelSenorArray[i] = values[cnt++];
        for (var i = 0; i < 12; i++) 
          airTemperatureSenorArray[i] = values[cnt++];
        for (var i = 0; i < 12; i++) 
          airHumiditySenorArray[i] = values[cnt++];
        for (var i = 0; i < 12; i++) 
          fanOneSpeedArray[i] = values[cnt++];
        for (var i = 0; i < 12; i++) 
          fanTwoSpeedArray[i] = values[cnt++];
        for (var i = 0; i < 31; i++) 
          autoFillDays[i] = values[cnt++];
        arrayCount = values[cnt++]; // 130 cnt
        // profile names
        profileOneName = values[cnt++];
        profileTwoName = values[cnt++];
        profileThreeName = values[cnt++];
        profileFourName = values[cnt++];
        profileFiveName = values[cnt++];
        // Light page
        onTimeHour = values[cnt++];
        onTimeMin = values[cnt++];
        offTimeHour = values[cnt++];
        offTimeMin = values[cnt++];
        lightMode = values[cnt++];
        // sensors
        ecSensor = values[cnt++];
        tdsSensor = values[cnt++];
        phSensor = values[cnt++];
        co2Sensor = values[cnt++];
        waterTemperatureSensor = values[cnt++];
        waterLevelSensor = values[cnt++];
        airTemperatureSensor = values[cnt++];
        airHumiditySensor = values[cnt++];
        // fans
        fanOneSpeed = values[cnt++];
        fantwoSpeed = values[cnt++];
        fanOneMinSpeed = values[cnt++];
        fanOneMaxSpeed = values[cnt++];
        fanTwoMinSpeed = values[cnt++];
        fanTwoMaxSpeed = values[cnt++];
        fansControlTemp = (values[cnt++] == "0" ? false : true);
        fansControlHum = (values[cnt++] == "0" ? false : true);
        fanOneFixedSpeed = (values[cnt++] == "0" ? false : true);
        fanTwoFixedSpeed = (values[cnt++] == "0" ? false : true);
        // user targets
        targetMinAirTemp = values[cnt++];
        targetMaxAirTemp = values[cnt++];
        airTempOffset = values[cnt++];
        targetMinHumidity = values[cnt++];
        targetMaxHumidity = values[cnt++];
        humidityOffset = values[cnt++];
        targetMinTds = values[cnt++];
        targetMaxTds = values[cnt++];
        tdsOffset = values[cnt++];
        targetCo2 = values[cnt++];
        co2Offset = values[cnt++];
        roomWidth = values[cnt++];
        roomLength = values[cnt++];
        roomHeight = values[cnt++];
        co2Flowrate = values[cnt++];// // to do note:"in liters per minute"
        co2CheckTimeMinute = values[cnt++];
        co2CheckTimeHour = values[cnt++];
        disableFansTimer = values[cnt++];
        manGasTimeEnabled = (values[cnt++] == "0" ? false : true);
        manualGasDurationTime = values[cnt++];
        co2ControlDisabled = (values[cnt++] == "0" ? false : true);
        targetMinPh = values[cnt++];
        targetMaxPh = values[cnt++];
        phOffset = values[cnt++];
        targetMinEc = values[cnt++];
        targetMaxEc = values[cnt++];
        ecOffset = values[cnt++];
        targetMinWaterTemp = values[cnt++];
        targetMaxWaterTemp = values[cnt++];
        waterTempOffset = values[cnt++];
        targetMinWaterHeight = values[cnt++];
        targetMaxWaterHeight = values[cnt++];
        waterHeightOffset = values[cnt++];
        // max mins
        minPh = values[cnt++];
        maxPh = values[cnt++];
        minCo2 = values[cnt++];
        maxCo2 = values[cnt++];
        minEc = values[cnt++];
        maxEc = values[cnt++];
        minWaterTemp = values[cnt++];
        maxWaterTemp = values[cnt++];
        minWaterLevel = values[cnt++];
        maxWaterLevel = values[cnt++];
        minTds = values[cnt++];
        maxTds = values[cnt++];
        minAirTemp = values[cnt++];
        maxAirTemp = values[cnt++];
        minHumidity = values[cnt++];
        maxHumidity = values[cnt++];
        minFanOneSpeed = values[cnt++];
        maxFanOneSpeed = values[cnt++];
        minFanTwoSpeed = values[cnt++];
        maxFanTwoSpeed = values[cnt++];
        // dosers
        numDosers = values[cnt++];
        doserOneMode = values[cnt++];// EC / PH + / Off
        doserTwoMode = values[cnt++];// EC / PH + / Off
        doserThreeMode = values[cnt++];// EC / PH + / Off
        doserFourMode = values[cnt++]; // EC / PH - / Off
        doserFiveMode = values[cnt++];// EC / PH + / Off
        doserSixMode = values[cnt++]; // EC / PH - / Off
        doserOneMills = values[cnt++];
        doserTwoMills = values[cnt++];
        doserThreeMills = values[cnt++];
        doserFourMills = values[cnt++];
        doserFiveMills = values[cnt++];
        doserSixMills = values[cnt++];
        //doserOneSpeed = values[cnt++];
        //doserTwoSpeed = values[cnt++];
        //doserThreeSpeed = values[cnt++];
        //doserFourSpeed = values[cnt++];
        //doserFiveSpeed = values[cnt++];
        //doserSixSpeed = values[cnt++];
        dosingInt = values[cnt++];
        swapInterval = values[cnt++];
        refillDoserOneMills = values[cnt++];
        refillDoserTwoMills = values[cnt++];
        refillDoserThreeMills = values[cnt++];
        refillDoserFourMills = values[cnt++];
        refillDoserFiveMills = values[cnt++];
        refillDoserSixMills = values[cnt++];
        autoFillHour = values[cnt++];
        autoFillMinute = values[cnt++];
        // warnings
        tdsErrorMargin = values[cnt++];
        ecErrorMargin = values[cnt++];
        phErrorMargin = values[cnt++];
        co2ErrorMargin = values[cnt++];
        waterTempErrorMargin = values[cnt++];
        waterHeightErrorMargin = values[cnt++];
        airTempErrorMargin = values[cnt++];
        humidityErrorMargin = values[cnt++];
        disablePpmWarn = (values[cnt++] == "0" ? false : true);
        disableEcWarn = (values[cnt++] == "0" ? false : true);
        disableCo2Warn = (values[cnt++] == "0" ? false : true);
        disablePhWarn = (values[cnt++] == "0" ? false : true);
        disableWaterTempWarn = (values[cnt++] == "0" ? false : true);
        disableWaterHeightWarn = (values[cnt++] == "0" ? false : true);
        disableAirTempWarn = (values[cnt++] == "0" ? false : true);
        disableHumidityWarn = (values[cnt++] == "0" ? false : true);
        // envriomental control
        disableDrainAndRefill = (values[cnt++] == "0" ? false : true);
        // DEVICE
        convertToF = (values[cnt++] == "0" ? false : true);
        convertToInchs = (values[cnt++] == "0" ? false : true);
        convertToTds = (values[cnt++] == "0" ? false : true);
        // system
        ppmErrorState = values[cnt++];
        ecErrorState = values[cnt++];
        phErrorState = values[cnt++];
        co2ErrorState = values[cnt++];;
        waterTempErrorState = values[cnt++];
        waterLevelErrorState = values[cnt++];
        airTempErrorState = values[cnt++];
        humidityErrorState = values[cnt++];
        
        userProfile = values[cnt++];
        loadProfileNum = userProfile;
        saveProfileNum = userProfile; 

        setPage(guiPageNumber, true);  
        connectionTime = new Date().getTime();
      }
      else {
        console.log("Error: expected 286 variables, received " + values.length);
        console.log("Variables string: " + this.responseText);
      }
    }
  };
  xhttp.open("GET", "getData", true);
  xhttp.send();
}

function setVal(cmd, val) {
  console.log("Sending command: /setValue?cmd="+cmd+"&val="+ val);
  var command = "/setValue?cmd="+cmd+"&val="+val;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", command, false);
  xhr.send();
}
</script>
</body>
</html>
)=====";
