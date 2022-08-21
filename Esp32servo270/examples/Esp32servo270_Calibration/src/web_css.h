//javascripts

#ifndef _WEB_CSS_H
#define _WEB_CSS_H
const char g_HTML_Web_CSS[] PROGMEM = R"=====(
	div{margin:0.5rem;}
.slidecontainer {
  width: 100%;
}

.slider {
  -webkit-appearance: none;
  width: 80%;
  height: 25px;
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px;
  height: 25px;
  background: #04AA6D;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 25px;
  background: #04AA6D;
  cursor: pointer;
}
fieldset {
  background-color: #eeeeee;
}
input[type=button] {padding:5px 15px; background:#ccc; border:0 none;
    cursor:pointer;
    -webkit-border-radius: 5px;
    border-radius: 5px;
    margin:10px;
    border: 1px solid #111111;}
legend {
  background-color: gray;
  color: white;
  padding: 5px 10px;
  
}
label {

display: block;
width: 300px;
float: left;
}
label:after { content: ": " }
.title{width:10rem; display:inline-box}

.overlay {
        position: absolute;
        display: none;
        top: 0;
        right: 0;
        bottom: 0;
        left: 0;
        background: rgba(0, 0, 0, 0.8);
        z-index: 2;
      }

      .popup {
        position: absolute;
        width:70%;
        height: 50%;
        top: 25%;
        left: 25%;
        text-align: left;
        background: white;
        padding:1.5em;
      }
     
      .popup h3 {
        font-size: 15px;
        height: 50px;
        line-height: 50px;
        color: black;
      }
      .CloseIcon{
        cursor: pointer;
        color:red;
        float:right;
      }
      pre, code {
  font-family: monospace, monospace;
}
pre {
  overflow: auto;
}
pre > code {
  display: block;
  padding: 1rem;
  word-wrap: normal;
}
)=====";
#endif