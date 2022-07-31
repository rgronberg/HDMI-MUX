#! /bin/env python3

"""
This is a test server to work on developing the HTML/CSS
without uploading the littleFS to the ESP everytime
"""

import http.server

selectedPort = 1

class RequestHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        global selectedPort

        if self.path == '/favicon.ico':
            self.send_response(200)
            self.send_header('content-type', 'image/x-icon')
            self.end_headers()

            with open('favicon.ico', 'rb') as favicon:
                self.wfile.write(favicon.read())
                return

        # Handle JS/CSS content
        if self.path == '/static/main.js':
            self.send_response(200)
            self.send_header('content-type', 'text/javascript')
            self.end_headers()

            with open('main.js') as mainJs:
                self.wfile.write(mainJs.read().encode('utf8'))
                return

        if self.path == '/static/bootstrap.min.css':
            self.send_response(200)
            self.send_header('content-type', 'text/css')
            self.send_header("Content-Encoding", "gzip")
            self.end_headers()

            with open('bootstrap.min.css.gz', 'rb') as minCss:
                self.wfile.write(minCss.read())
                return

        if self.path == '/static/bootstrap.min.css.map':
            self.send_response(200)
            self.send_header('content-type', 'text/json')
            self.send_header("Content-Encoding", "gzip")
            self.end_headers()

            with open('bootstrap.min.css.map.gz', 'rb') as minCssMap:
                self.wfile.write(minCssMap.read())
                return

        # Handle text content
        if self.path == '/selected':
            self.send_response(200)
            self.send_header('content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(str(selectedPort).encode('utf8'))
            return

        # Handle HTML content
        self.send_response(200)
        self.send_header('content-type', 'text/html')
        self.end_headers()

        response = 'not found'
        if self.path == '/':
            with open('index.html') as indexHtml:
                response = indexHtml.read()

        elif self.path == '/port1':
            selectedPort = 1
            response = 'Port 1 has been enabled'

        elif self.path == '/port2':
            selectedPort = 2
            response = 'Port 2 has been enabled'

        elif self.path == '/port3':
            selectedPort = 3
            response = 'Port 3 has been enabled'

        elif self.path == '/port4':
            selectedPort = 4
            response = 'Port 4 has been enabled'

        elif self.path == '/next':
            selectedPort = (selectedPort % 4) + 1
            response = 'The next port has been enabled'

        elif self.path == '/reset':
            with open('reset.html') as resetHtml:
                response = resetHtml.read()

        elif self.path == '/reallyreset':
            # Need some JS here, but that can come later
            response = 'Settings have been reset'

        elif self.path == '/upgrade':
            response = 'Software upgrade initiated...'

        self.wfile.write(response.encode('utf8'))

def main():
    server_address = ('0.0.0.0', 8000)
    server = http.server.HTTPServer(server_address, RequestHandler)
    print('Server listening on port 8000')
    server.serve_forever()

if __name__ == '__main__':
    main()
