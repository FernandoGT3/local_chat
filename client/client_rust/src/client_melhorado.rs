use std::io::{self, Read, Write};
use std::net::TcpStream;
use std::thread;

fn read_messages(mut stream: TcpStream) {
    let mut buffer = [0; 1024];
    loop {
        match stream.read(&mut buffer) {
            Ok(bytes_read) if bytes_read > 0 => {
                let message = String::from_utf8_lossy(&buffer[..bytes_read]);
                println!("Received: {}", message);
            }
            Ok(_) => {
                // Connection closed by the server
                println!("Server closed the connection.");
                break;
            }
            Err(_) => {
                // Handle errors
                println!("Error reading from server.");
                break;
            }
        }
    }
}

fn main() -> io::Result<()> {
    let server_address = "127.0.0.1:8080";

    let mut stream = TcpStream::connect(server_address)?;
    println!("Connected to server at {}", server_address);

    // Clone the stream for reading messages
    let reader_stream = stream.try_clone()?;

    // Spawn a thread to read messages from the server
    thread::spawn(move || {
        read_messages(reader_stream);
    });

    // Read user input and send it to the server
    let mut input = String::new();
    loop {
        io::stdin().read_line(&mut input)?;
        stream.write_all(input.as_bytes())?;
        input.clear();
    }
}