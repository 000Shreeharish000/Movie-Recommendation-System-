const http = require('http');
const path = require('path');
const fs = require('fs');
const url = require('url');

const server = http.createServer((req, res) => {
    const reqUrl = url.parse(req.url, true);
    const pathname = reqUrl.pathname;

    if (pathname === '/') {
        // Serve the HTML file
        fs.readFile(path.join(__dirname, 'index.html'), 'utf-8', (err, data) => {
            if (err) {
                res.statusCode = 500;
                res.end('Error loading HTML file.');
                return;
            }
            res.statusCode = 200;
            res.setHeader('Content-Type', 'text/html');
            res.end(data);
        });
    } else if (pathname === 'main.cpp') {
        // Handle the CGI request (by invoking the C++ script)
        const { exec } = require('child_process');
        const user = reqUrl.query.username; // Extract the selected user from the query string

        exec(`./main.cpp?username=${user}`, (error, stdout, stderr) => {
            if (error || stderr) {
                res.statusCode = 500;
                res.end('Error running CGI script');
                return;
            }
            res.statusCode = 200;
            res.setHeader('Content-Type', 'text/html');
            res.end(stdout);  // Output the result of the CGI script
        });
    } else {
        res.statusCode = 404;
        res.end('Not Found');
    }
});

server.listen(8080, () => {
    console.log('Server running at http://localhost:8080/');
});
