const express = require('express');
const cors = require('cors');
const app = express();
const port = 3000;
const corsOptions = {
  origin: "*"
};
app.use(express.json());
app.use(cors(corsOptions));

let realTimeData = {id: false, message: "Nenhum dado ainda"}

app.post('/dados', (req, res) => {
  const dados = req.body;
  console.log('Dados recebidos:', dados);
  realTimeData = dados;	  
  res.sendStatus(200);
});
//To test /real-time route you can do curl -N localhost:3000/real-time
app.get("/real-time", (req, res)=>{
 const headers = {
 'Content-Type': 'text/event-stream',
 'Connection': 'keep-alive',
 'Cache-Control': 'no-cache'
 };

 res.writeHead(200 ,headers);
 setInterval(() => {
  res.write(JSON.stringify(realTimeData)); 
 }, 1000);


});


app.get("/get", (req, res)=>{
 res.status(200).send("Working!!");
})

app.listen(port, () => {
  console.log(`Servidor Node.js rodando na porta ${port}`);
});
