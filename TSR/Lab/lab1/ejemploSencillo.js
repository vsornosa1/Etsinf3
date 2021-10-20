// ejemploSencillo.js --------------------------------------------------
const http = require('http');

function dd(i) { return (i < 10 ? "0" : "") + i; }

const server = http.createServer(
    function (req, res) {
        res.writeHead(200, { 'Content-Type': 'text/html' });
        res.end('<marquee>Node y Http</marquee>');
        var d = new Date();
        console.log('alguien ha accedido a las ' +
            d.getHours() + ":" +
            dd(d.getMinutes()) + ":" +
            dd(d.getSeconds()));
    }).listen(8000)