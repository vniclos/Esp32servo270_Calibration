#ifndef _WEB_ROOT_H
#define _WEB_ROOT_H
#include <Arduino.h>

const char g_HTML_Web_root[] PROGMEM = R"=====(<!DOCTYPE html>
<head>
 <meta charset=UTF-8>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="X-UA-Compatible" content="ie=edge">
<title>Esp32 Servo callibration</title>

<link rel="stylesheet" href="css.css">
</head>
<body>

<div>

  	<fieldset>
  		<legend>Configuration servo</legend>
  		<h4>Acording datasheet of hardaware servo</h4>
  		<div>
  	  <label for="scnHardServoPin" class="fieldTitle">Pin</label> 
      <input id="scnHardServoPin" type="number" placeholder="2"  size:"2" onkeypress="return onlyNumberKey(event)" value="4" />
      <span id="scnHardServoPinMsg"></span>
        <button onclick="OpenModal('Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33. In some dev board pin 2 is atached to led blue')">Help</button>
      </div>
    
    
  		
  		<div>
      <label for="scnDatasheetMinMicroSeconds">Minimum pulse in microseconds</label> 
      <input id="scnDatasheetMinMicroSeconds" placeholder="500" type="number"  maxlength="3" size:"3" onkeypress="return onlyNumberKey(event)"  value="600"/>
      <button onclick="OpenModal('Start with datasshet value, later aujust fine. Usually usually {500 or 600}.<br/>Alway great than 270 and less than 700')">Help</button>
      <span id="scnDatasheetMinMicroSecondsMsg"></span>

      </div>
      
      <div>
      <label for="scnDatasheetMaxMicroSeconds">Maximum pulse in microseconds</label>  
      <input id="scnDatasheetMaxMicroSeconds" placeholder="2000" type="number"  maxlength="4"  size:"4"  onkeypress="return onlyNumberKey(event)"  value="2000"/>
      <button onclick="OpenModal('Start with datasshet value, later aujust fine. Usually 1500,200 or 2500<br/> Allways greater than 1499')">Help</button>
      <span id="scnDatasheetMaxMicroSecondsMsg"></span>
      </div>
     <div>
      <label for="scnDatasheetMaxDegree max Degree">Maximum turn in degrees</label>
      <input id="scnDatasheetMaxDegree" type="number" placeholder="180"  maxlength="3" size:"3" onkeypress="return onlyNumberKey(event)"  value="180"/>
       <button onclick="OpenModal('Start with datasshet value, later aujust fine. Usually one of this[90,180.270,270] degree.<br/>Note: Min=90, Max=270')">Help</button>
      <span id="scnDatasheetMaxDegreeMsg" ></span>
      </div>
    <div><b>Limitation movement in the installation</b>
    <button onclick="OpenModal('For use in specific installations where the freedom of movement is restricted,</br>This is usually caused by collisions with nearby parts.</br>The maximum and minimum position in degrees can be limited by software<br/>This limitation must be within the range of zero to the maximum that the servo can hardawre turn.')">Help</button>
    </div>
       	<div>
  	  <label for="scnMoveMinDegree" class="fieldTitle">Lowest position in degrees</label> 
      <input id="scnMoveMinDegree" type="number" placeholder="0" maxlength="3"  size:"3" onkeypress="return onlyNumberKey(event)" />
       <span id="scnMoveMinDegreeMsg"></span>
      </div>
       <div>
      <label for="scnMoveMaxDegree max Degree">Higest position in degrees</label>
      <input id="scnMoveMaxDegree" type="number" placeholder="180"  maxlength="3"  size:"3" onkeypress="return onlyNumberKey(event)" />
       <span id="scnMoveMaxDegreeMsg"></span>
      </div>
  	
  		<input id=ScnMoveToButon" type="button" value="Atach" onclick="jsAtach();">
  		<input id=ScnMoveToButon" type="button" value="Detach" onclick="jsDetach();">
    <div>
  		<span id="scnMsgConfiguration"></span>
      <div>
      <b>Usual values of servos model, but this can be change by manufacte</b>
      <table>
      <tr><td>Model</td> <td>degree</td><td>µS min</td><td>µSMax</td><td>Signal</td</tr>
      <tr><td>MG995</td ><td>180</td><td>500</td><td>2000</td><td>50Hz 20ms></tr>
   <tr><td>MG996r</td ><td>180</td><td>500</td><td>2500</td><td>50Hz 20ms></tr>
  		</fieldset>
  	
  		<br/><br/>
  		
  <fieldset>
  <legend>Testing movements</legend>		
  		
  		
<fieldset>
  <legend>Move degree</legend>
  <input id="scnMoveToSliderDegree" type="range" min="0" max="270" value="50" class="slider"    oninput="showVal(this.value,'scnMoveToValueDegree')"/>

 
  <input id=ScnMoveToButonDegree" type="button" value="Move" onclick="jsMove('scnMoveToSliderDegree','scnMoveToDegreeMsg')">
   Degree <span id="scnMoveToValueDegree"></span>
  <span id="scnMoveToDegreeMsg"></span>
</fieldset>

<br/>
<fieldset>
  <legend>Move microseconds</legend>
  <input id="scnMoveToSliderMicro" type="range" min="550" max="2000" value="1000" class="slider" 
  oninput="showVal(this.value,'scnMoveToValueMicro')"
  onchange="showVal(this.value,'scnMoveToValueMicro')">
  
  <input id=ScnMoveToButon" type="button" value="Move" onclick="jsMove('scnMoveToSliderMicro','scnMsgMoveMicro')">
  MicroSeg <span id="scnMoveToValueMicro"></span>
  <span id="scnMsgMoveMicro"></span>
</fieldset>
<fieldset>
  <legend>Testing turn rigth left</legend>
  <input id=ScnBtnTurnMicro" type="button" value="Turn steeps in  microseconds" onclick="jsTurn('micro','scnMsgTurn')">
    <input id=ScnBtnTurnDegree" type="button" value="Turn steeps in degree" onclick="jsTurn('degree','scnMsgTurn')">
<span id="scnMsgTurn"></span>
</fieldset>
<br/>
<fieldset>
	 <legend>Get example based in your settings </legend>
   <input id=scnBtnTemplate" type="button" value="Update template code" onclick="jsTemplate()">
   <pre><code id="scnMsgTemplate"></code></pre>
</fieldset>
</div>
<div class="overlay" id="overlay"><div class="popup"><div onclick="CloseModal()" class="CloseIcon">&#10006;</div><p id="scnModalMsg"></p></div></div></div>
<script>
window.addEventListener("load", function() {
  document.getElementById("scnMoveToValueDegree").innerHTML=  document.getElementById("scnMoveToSliderDegree").value;
  document.getElementById("scnMoveToValueMicro").innerHTML=  document.getElementById("scnMoveToSliderMicro").value;
});
	function showVal(newVal,scnShow){
    console.log(scnShow+"="+newVal);
 
  document.getElementById(scnShow).innerHTML=newVal;
}

</script>
 <script src="js.js"></script> 
</body>
</html>
)=====";
#endif