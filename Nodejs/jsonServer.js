var express = require('express');

var app = express();

app.get('/json1', function(req, res){
  var json = JSON.stringify({ uno: 1, dos : 2 }, null, '\t');
  res.send(json);
});

app.get('/json2', function(req, res){
     var body = {
        "sleep" : "20000",
	"image" : "0xff 0xaa 0xbb"
      };
  var json = JSON.stringify(body);
  res.send(json);
});

app.listen(8000);
