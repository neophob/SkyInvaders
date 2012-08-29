var express = require('express');

var app = express();

app.get('/json2', function(req, res){
  var json = JSON.stringify({ uno: 1, dos : 2 }, null, '\t');
  res.send(json);
});

app.listen(8000);
