const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<head>
<meta name="mobile-web-app-capable" content="yes">
<link rel="shortcut icon" type="image/png" href="https://upload.wikimedia.org/wikipedia/commons/8/8a/Recetteplus_favicon.png"/>
<title>AutoBlindz REMOTE</title>
</head>
<style>
body {background-color: #2d2d2d;}
button   {font-family: proxima-nova, monospace;background-color: #2d2d2d;cursor: pointer; width: 90%;height:130px; margin:0.3em;font-size:3em; font-family: Arial;border-color:#23efe0;border-radius:20px;color:#23efe0;border-width:thick;}
button:hover {background: #5b5b5b; }
a    {color: #ccc; text-decoration:none;font-size:2em;}

h1 {color: #fff; font-size:4em;}
.input-range {  w
  margin: 0;
  transform-origin: 370px 370px;
  transform: rotate(90deg);
    -webkit-appearance: none;
    width: 700px;
    height: 650px;
    border-radius: 5px;   
    background: #424040;
    outline: none;
    opacity: 0.7;
    -webkit-transition: .2s;
    transition: opacity .2s;}
    
.input-range::-webkit-slider-thumb {
    -webkit-appearance: none; /* Override default look */
    appearance: none;
    width: 25px; /* Set a specific slider handle width */
    height: 700px; /* Slider handle height */
    background: #23efe0; /* Green background */
    cursor: pointer; /* Cursor on hover */
}
.slider-wrapper { display: inline-block;
  width: 300px;
  height: 700px;
  padding: 2em; 
  float:left;}

  #button-wrapper {
    float:right;
  }
  #sliderValue {float:left;}
  #data {
    position: absolute;
    top;10px;
    left:150px;
    text-decoration: italic;
    font-size:1em; color: #ccc; 
    
  }
</style>
<script>
function loadData(url, callback){
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function(){
 if(this.readyState == 4 && this.status == 200){
 updateData.apply(xhttp);
 }
};
xhttp.open("PUT", url, true);
xhttp.send();
getTargetPos();
}
function updateData(){
 document.getElementById("data").innerHTML = this.responseText;
 

}
function getTargetPos() {
 
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function(){
 if(this.readyState == 4 && this.status == 200){
 updateSliderValue.apply(xhttp);
 }
};
xhttp.open("GET", "/getTargetPosition", true);
xhttp.send();

}

function updateSliderValue() {
 
   document.getElementById("sliderInput").value = this.responseText;
}
loadData("/getCurrentPosition");

function updateSlider(sliderPosition) {
 var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function(){
 if(this.readyState == 4 && this.status == 200){
 updateData.apply(xhttp);
 }
};
xhttp.open("PUT", "/setTargetPosition?pos="+sliderPosition, true);
xhttp.send();
 
}
window.document.onload = function(e){ 
  loadData("/getPosition","updateSliderValue");
  }
</script>
<html>
<body>
<center>
<h1>AUTOBLINDZ REMOTE</h1>

 
 <div id="button-wrapper">
 <button onClick="loadData('/setTargetPosition?pos=0')">Blinds Up</button>
 <button onClick="loadData('/setTargetPosition?pos=11')">Blinds down</button>
 <button onClick="loadData('/stepUp')">1 Up</button>
  <button onClick="loadData('/stepDown')">1 Down</button>
<button onClick="loadData('/setUp')">Set Up pos</button>
<button onClick="loadData('/setDown')">Set Down pos</button>

</div>
<div id="data"></div>
</center>

</body>
</html>
)=====";
