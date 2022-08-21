// javascripts
//  validator https://jshint.com/
// https://codebeautify.org/jsviewer
#ifndef _WEB_JS_H
#define _WEB_JS_H
const char g_HTML_Web_JS[] PROGMEM = R"=====(

//..................
function fncGetHtml(pDivTarget, pURL) {
 console.log("pDivTarget=" + pDivTarget + "\n pURL= " + pURL);
 var xhttp;
 xhttp = new XMLHttpRequest();
 xhttp.onreadystatechange = function() {
  if (xhttp.readyState == 4 && xhttp.status == 200) {
  
     document.getElementById(pDivTarget).innerHTML = xhttp.responseText ;

  }

 };
 xhttp.open("GET", pURL, true);
 xhttp.send();
}
 

function onlyNumberKey(evt) {
          
        // Only ASCII character in that range allowed
        var ASCIICode = (evt.which) ? evt.which : evt.keyCode
        if (ASCIICode > 31 && (ASCIICode < 48 || ASCIICode > 57))
            return false;
        return true;
    }
function jsIsFieldEmptyOrSpaces(idField){
	let str=document.getElementById(idField).value;
    return str === null || str.match(/^ *$/) !== null;
}
function jsAtachValidationField(idField,idFieldMsg,iValidMin,iValidMax,msg)
  {
    let bOk=true;
  let val=0;
  document.getElementById(idFieldMsg).style.color = 'black'
  document.getElementById(idFieldMsg).innerHTML="";
  document.getElementById(idField).value;
  val=document.getElementById(idField).value;
  val=1*val.replace(/\s/g, '') ;
  va2=1*val;
  val=document.getElementById(idField).value=va2;

console.log(idField+"="+va2);
 console.log("Min="+iValidMin);
      console.log("Max="+iValidMax);

  if (jsIsFieldEmptyOrSpaces(idField)) {bOk=false;}

  if(va2>=iValidMin && va2<=iValidMax){ 
    console.log("true")
    }
    else
    {
      bOk=false;
    document.getElementById(idFieldMsg).innerHTML="Error:"+msg;
	  document.getElementById(idFieldMsg).style.color = 'red';

  	}
    	console.log(bOk);
    	console.log("------------");
	return bOk;	
}
    
function jsAtachValidationForm(){
	let bOk=true;
  let idField="";
  let idFieldMsg="";
  let msg="";
  let iValidMin=0;
  let iValidMax=0;
  //.....................
  idField="scnHardServoPin";
  idFieldMsg="scnHardServoPinMsg";
  iValidMin=1;
  iValidMax=33;
  msg="Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33";
  if(!jsAtachValidationField(idField,idFieldMsg,iValidMin,iValidMax,msg)){bOk=false;}
	
  //.....................
  idField="scnDatasheetMaxDegree";
  idFieldMsg="scnDatasheetMaxDegreeMsg";
  iValidMin=90;
  iValidMax=270;
  msg="Usually one of this[90,180.270,270] degree. Note: Min=90, Max=270";
  if(!jsAtachValidationField(idField,idFieldMsg,iValidMin,iValidMax,msg)){bOk=false;}




	//.....................
  idField="scnDatasheetMinMicroSeconds";
  idFieldMsg="scnDatasheetMinMicroSecondsMsg";
  iValidMin=400;
  iValidMax=701;
  msg="Usually usually {500 or 600} alway great than 399 and less than 700";
  if(!jsAtachValidationField(idField,idFieldMsg,iValidMin,iValidMax,msg)){bOk=false;}

//.....................
  idField="scnDatasheetMaxMicroSeconds";
  idFieldMsg="scnDatasheetMaxMicroSecondsMsg";
  iValidMin=1499;
  iValidMax=2500;
  msg="Usually 1500,200 or 2500, allways greater than 1499";
  if(!jsAtachValidationField(idField,idFieldMsg,iValidMin,iValidMax,msg)){bOk=false;}

//...........................
	idField="scnMoveMinDegree";
  idFieldMsg="scnMoveMinDegreeMsg";
  iValidMin=0;
  iValidMax=   document.getElementById('scnDatasheetMaxDegree').value; 
  msg="Alwais lessthan to Maximum turn in degrees and lessthan  to Hightest position in degrees";
  if(jsIsFieldEmptyOrSpaces( idField))
  {
  
  	document.getElementById(idField).value=0;
  	}
  if(!jsAtachValidationField(idField,idFieldMsg,iValidMin,iValidMax,msg)){bOk=false;}
  
    iValidMin=  document.getElementById("scnMoveMinDegree").value; ;
//.........................................
	idField="scnMoveMaxDegree";
	idFieldMsg="scnMoveMaxDegreeMsg";
	idFieldMax="scnDatasheetMaxDegree";
  iValidMax=   document.getElementById(idFieldMax).value; 
   
  if(jsIsFieldEmptyOrSpaces( idField))
  {
    console.log("aqui 1");
 	document.getElementById(idField).value=iValidMax;
  	}
   
  msg="this degree value must be greater than or equal to the minimum and less than or equal to the maximum";


  if(!jsAtachValidationField(idField,idFieldMsg,iValidMin,iValidMax,msg)){bOk=false;}


	
	return bOk;
}
function jsDetach()
{
let pURL="/detach";
let pDivTarget="scnMsgConfiguration";
fncGetHtml(pDivTarget, pURL);

}
function jsAtach(){
if (!jsAtachValidationForm()){return ;}
let pDivTarget="scnMsgConfiguration";
let pURL="/atach?Hwpin=";
pURL=pURL+document.getElementById("scnHardServoPin").value;
pURL=pURL+"&HwMin="+document.getElementById("scnDatasheetMinMicroSeconds").value;
pURL=pURL+"&HwMax="+document.getElementById("scnDatasheetMaxMicroSeconds").value;
pURL=pURL+"&HwMaxD="+document.getElementById("scnDatasheetMaxDegree").value;
pURL=pURL+"&SoMinD="+document.getElementById("scnMoveMinDegree").value;
pURL=pURL+"&SoMaxD="+document.getElementById("scnMoveMaxDegree").value;
document.getElementById("scnMsgConfiguration").innerHTML=pURL;
let iSliderMax=document.getElementById("scnMoveMaxDegree").value;
if(document.getElementById("scnDatasheetMaxDegree").value>document.getElementById("scnMoveMaxDegree").value);
{
  iSliderMax=document.getElementById("scnMoveMaxDegree").value;
}
document.getElementById("scnMoveToSliderDegree").max=iSliderMax;
document.getElementById("scnMoveToSliderMicro").min=document.getElementById("scnDatasheetMinMicroSeconds").value;
document.getElementById("scnMoveToSliderMicro").max=document.getElementById("scnDatasheetMaxMicroSeconds").value;
fncGetHtml(pDivTarget, pURL);
}
function jsTemplate ()
{
  jsAtach();
 let pDivTarget="scnMsgTemplate";
  let pURL="/template";
 fncGetHtml(pDivTarget, pURL) ;
}
function jsMove(byIdValue,scnMsgById)
{
  let pURL="/move?moveto="+document.getElementById(byIdValue).value;
 fncGetHtml(scnMsgById, pURL) ;
	}
  //...................
function jsTurn(mode,pDivTarget)
{
let pURL="";
if(mode=="micro"){ pURL="turnmicro";}
if (mode="degree"){ pURL="turndegree";}
if (pURL!=""){fncGetHtml(pDivTarget, pURL);}
}
function OpenModal(msg) {
  let element = document.getElementById('overlay')
  document.getElementById('scnModalMsg').innerHTML=msg;
  element.style.display = 'block'
}
function CloseModal() {
  let element = document.getElementById('overlay')
  element.style.display = 'none'
}
)=====";
#endif