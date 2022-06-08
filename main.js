//NO HACE FALTA MODIFICAR ESTE CÓDIGO.

var express = require('express');
var app = express();
var server = require('http').Server(app);
const io = require('socket.io')(server);
// Puras librerías y la constante io para el server.

app.use(express.static('public')); // Usamos la carpeta public para mostrar la página WEB.

io.on('connection', function (socket) { // Verifica si se conecto alguien.
  console.log('Nueva Dispositivo Conectado');
  socket.on('telemetria', function(data) // Escucha los clientes que manden un JSON con la orden 'telemetria'
  {
    console.log(data); // Imprime lo que le manden para reconocer lo que se manda.
    io.emit('telemetria', data);// Se le manda a todos los clientes un JSON 'telemetria'.
  });

  socket.on('controlar_ESP', function(data) // Aquí se mandan las órdenes al ESP32 que este conectado al server.
  {
    console.log(data); // Se imprime la órden recibida.
    io.emit('controlar_ESP', data); // Manda al ESP32 una órden en formato JSON.
  });

});

server.listen(80, function(){
    console.log("Servidor corriendo en el puerto 5001.")
});
