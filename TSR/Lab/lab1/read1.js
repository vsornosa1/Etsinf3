// read1.js -------------------------------------------------------------------------
const fs = require('fs');
fs.readFile('/etc/hosts', 'utf8', function (err, data) {
    if (err) {
        return console.log(err);
    }
    console.log(data);
})