const express = require("express");
const app = express();
const {resolve} = require("node:path");
const cors = require("cors");
const {createServer} = require("node:http");
const {Server} = require("socket.io");

app.use(express.static(resolve(__dirname, "public")));
app.use(express.json());
app.use(cors({origin: "*"}));

const server = createServer(app);
const io = new Server(server);

io.on("connection", (socket)=>{
  console.log("Client connected");
})

app.get("/", (_,r)=> r.sendStatus(200));

app.post("/dados", (req, res)=>{
  const data = req.body;
  console.log(data);
  io.emit("data-api", data);
  res.status(200).send(data);
});

server.listen(3002, ()=>{
  console.log("Server is running at 3002")
})