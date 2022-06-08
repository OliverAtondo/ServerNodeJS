var express = require('express');
var app = express();
var server = require('http').Server(app);
const io = require('socket.io')(server);

app.use(express.static('public'));

var usuarios = [];

io.on('connection', function (socket) {
  console.log('Nueva Dispositivo Conectado');
  socket.on('telemetria', function(data)
  {
    console.log(data);
    io.emit('telemetria', data);
  });

  socket.on('controlar_ESP', function(data)
  {
    console.log(data);
    io.emit('controlar_ESP', data);
  });

});

server.listen(80, function(){
    console.log("Servidor corriendo en el puerto 5001.")
});
