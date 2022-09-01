import express from "express";
import path from "path"
import bodyParser from "body-parser";
import { SerialPort, ReadlineParser } from "serialport";

const PORT = 'COM3'
const serialport = new SerialPort({ path: PORT, baudRate: 9600 })
const parser = serialport.pipe(new ReadlineParser({ delimiter: '\r\n' }))
var lastPost = 'NotStarted'
parser.on('data', (data)=>{
  console.log(data);
  lastPost = data;
})

const postString = (req, res) => {
  let code = req.params.str;
  serialport.write(code)
  serialport.write('\n')
  console.log(code)
  res.status(200).send({ message: "Enviado com sucesso"})
}

const sendString = (req, res) => {
  res.status(200).send(lastPost)
}

const routes = (app) => {
    app.route('/').get((req, res) => {
      res.status(200).sendFile(path.join(path.resolve()+'/public/GUI.html'));
    })
    app.use(
      express.static("public"),
      express.json(),
      bodyParser.urlencoded({
        extended: true
      }),
      express.Router().post('/:str',postString),
      express.Router().get('/str',sendString)
    )
  }
  
const app = express();
app.use(express.json())
routes(app);



export default app