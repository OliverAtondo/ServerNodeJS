var socket = io.connect('172.16.13.177:80', {'forceNew':true})
let usuarios = ["17211817","20212712","19212367","19212368","19212369","19212371","19212375","19212377","19212378","20211961","19212384","19212386","19212387","20210781","18211544","16212301","19210606","18211548","19212402","19212405","19212407","19212409","19212418","17211858","18210146","19212421","19212422","19212426","18212732","16212260","20210827","19212435","19212436","20212523","19212439","20212987","19212443","19212444","19212446","19212447"];

/*for (var i = 0; i < usuarios.length; i++) {
  document.getElementById("myTable").insertRow(-1).innerHTML = '<td>'+usuarios[i]+'</td><td>OFF</td><td>OFF</td><td>OFF</td>';
}*/

socket.on('telemetria', function(data){
    console.log(data.id);
    document.getElementById(data.id).innerHTML = '<td>'+data.id+'</td><td>'+data.temperatura+'<button onclick="temp('+data.id+', this)">'+data.temperatura+'</button></td><td>'+data.humedad+' <button onclick="hum('+data.id+', this)">'+data.humedad+'</button> </td><td>'+data.luz+' <button onclick="encender('+data.id+', this)">'+data.luz+'</button> </td>';
});

function encender(num, element) {
  if (element.textContent == 'OFF') {
    element.style.color = 'blue';
    socket.emit("controlar_ESP", "{'id':"+num+",'command':'LIGHT_ON'}");
    element.textContent = 'ON';
  } else if (element.textContent == 'ON'){
    element.style.color = 'red';
    socket.emit("controlar_ESP", "{'id':"+num+",'command':'LIGHT_OFF'}");
    element.textContent = 'OFF';
  }
}

function temp(num, element) {
  if (element.textContent == 'OFF') {
    element.style.color = 'blue';
    socket.emit("controlar_ESP", "{'id':"+num+",'command':'TEMPERATURE_ON'}");
    element.textContent = 'ON';
  } else {
    element.style.color = 'red';
    socket.emit("controlar_ESP", "{'id':"+num+",'command':'TEMPERATURE_OFF'}");
    element.textContent = 'OFF';
  }
}

function hum(num, element) {
  if (element.textContent == 'OFF') {
    element.style.color = 'blue';
    socket.emit("controlar_ESP", "{'id':"+num+",'command':'HUMIDITY_ON'}");
    element.textContent = 'ON';
  } else {
    element.style.color = 'red';
    socket.emit("controlar_ESP", "{'id':"+num+",'command':'HUMIDITY_OFF'}");
    element.textContent = 'OFF';
  }
}
