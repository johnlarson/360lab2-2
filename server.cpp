#include "server.h"

#define ERROR -1
#define QUEUE_SIZE 5

struct Request {
	string method;
	string path;
	string version;
};

void setUpServerSocket(int &sSocket, struct sockaddr_in &address, int port);
struct Request getRequest(int pSocket);
void respond(int pSocket, string version, string path);
string getContentType(string path);
string joinPath(string parent, string child);
string get404();
void write(int pSocket, string msg);
void writeLine(int pSocket, string msg);
void writeLine(int pSocket);

void runServer(int port, string root) {
	int sSocket;
	struct sockaddr_in address;
	setUpServerSocket(sSocket, address, port);
	int addressSize = sizeof(struct sockaddr_in);
	while(1) {
		int pSocket = accept(sSocket, (struct sockaddr*)&address, (socklen_t*)&addressSize);
		struct Request request = getRequest(pSocket);
		string path = joinPath(root, request.path);
		respond(pSocket, request.version, path);
		if(close(pSocket) == ERROR) {
			errorOut("Couldn't close message socket");
		}
	}
	if(close(sSocket) == ERROR) {
		errorOut("Couldn't close server socket");
	}
}

void setUpServerSocket(int &sSocket, struct sockaddr_in &address, int port) {
	sSocket = socket(AF_INET, SOCK_STREAM, 0);
        if(sSocket == ERROR) {
               errorOut("Couldn't start server socket");
        }   
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        address.sin_family = AF_INET;
        if(bind(sSocket, (struct sockaddr*)&address, sizeof(address)) == ERROR) {
                errorOut("Couldn't bind address to socket");
        }   
        int addressSize = sizeof(struct sockaddr_in);
        getsockname(sSocket, (struct sockaddr*)&address, (socklen_t*)&addressSize);
        if(listen(sSocket, QUEUE_SIZE) == ERROR) {
		errorOut("Couldn't listen on socket");
        }   

}

struct Request getRequest(int pSocket) {
	struct Request request;
	string line = GetLine(pSocket);
	stringstream st;
	st.str(line);
	st >> request.method >> request.path >> request.version;
	cout << "request.method: " << request.method << endl;
	cout << "request.path: " << request.path << endl;
	cout << "request.version: " << request.version << endl;
	return request;
}

void respond(int pSocket, string version, string path) {
	struct stat filestat;
	//string contentType = getContentType(path);
	if(stat(path, filestat) == ERROR) {
		writeLine(pSocket, version + " 404 NOT FOUND");
		writeLine(pSocket, "Content-Type: text/html");
		string body = get404();
		string contentLength = to_string(body.length());
		writeLine(pSocket, "Content-Length: " + contentLength);
		writeLine(pSocket);
		write(pSocket, body);
	} else if(S_ISREG(filestat.st_mode)) {
		writeLine(pSocket, version + " 200 OK");
		writeLine(pSocket, "Content-Type: " + getContentType(path));
		string contentLength = to_string(filestat.st_size);
		
	}
		
}

string getContentType(string path) {
	int dotIndex = path.find_last_of(".");	
	int start = dotIndex + 1;
	string ext = path.substr(start);
	if(ext == "txt") {
		return "text/plain";
	} else if(ext == "html") {
		return "text/html";
	} else if(ext == "jpg") {
		return "image/jpg";
	} else if(ext == "gif") {
		return "image/gif";
	} else {
		return "text/plain";
	}
}

string joinPath(string parent, string child) {
	if(parent.back() == '/') parent.pop_back();
	if(child.front() == '/') child.erase(0, 1);
	return parent + "/" + child;
}

void writeLine(int pSocket, string msg) {
	write(pSocket, msg + "\r\n");
}

void writeLine(int pSocket) {
	writeLine(pSocket, "");
}

void write(int pSocket, string msg) {
	write(pSocket, msg.c_str(), msg.length());
}

string get404() {
	return "<!DOCTYPE html><html><head></head><body>Sorry, braw, could find that for ya.</body></html>";
}