const http = require('http')
const url = require('url')
const client = require('prom-client')

// MY VARIABLES:
var breakings=[0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0];
var livingroomBreaks=[1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
var bedroomBreaks=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
var gardenBreaks=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0];

// Seconds
var index=0;
var burglaryLastStatus = 0;
var breakLastStatus = 0;


// Create a Registry which registers the metrics
const register = new client.Registry()
// Add a default label which is added to all metrics
register.setDefaultLabels({
  app: 'house-alert'
})
// Enable the collection of default metrics
client.collectDefaultMetrics({ register })

// ---> Living Room
const metricLivingroomBreak = new client.Gauge({
  name: 'livingroom_break',
  help: 'is someone tries to break to the living room'
});
register.registerMetric(metricLivingroomBreak)
// ---> Bed Room
const metricbedroomBreak = new client.Gauge({
  name: 'bedroom_break',
  help: 'is someone tries to break to the bed room'
});
register.registerMetric(metricbedroomBreak)
// ---> Garden
const metricGardenBreak = new client.Gauge({
  name: 'garden_break',
  help: 'is someone tries to break to the garden'
});
register.registerMetric(metricGardenBreak)
// ---> Active Devices
const numberOfActiveDevices = new client.Gauge({
  name: 'number_of_active_devices',
  help: 'number of active sensors'
});
register.registerMetric(numberOfActiveDevices)
// ---> Defective Devices
const numberOfDefectiveDevices = new client.Gauge({
  name: 'number_of_defective_devices',
  help: 'number of defective sensors'
});
register.registerMetric(numberOfDefectiveDevices)
// ---> Is Safe: does all the sensors are quiet
const metricIsSafe = new client.Gauge({
	name: 'is_safe',
	help: 'Example of a gauge'
});
register.registerMetric(metricIsSafe)
// ---> Number of Total Burglaries
const numOfTotalBurglaries = new client.Counter({
    name: 'num_of_total_burglaries',
    help: 'metric_help',
});
register.registerMetric(numOfTotalBurglaries)

const numOfBreaks = new client.Counter({
    name: 'num_of_breaks',
    help: 'metric_help',
});
register.registerMetric(numOfBreaks)

// Define the HTTP server
const server = http.createServer(async (req, res) => {

  var isBreak = breakings[(index + breakings.length) % breakings.length];
  // ---> Living Room
  var lvBreakStatus = livingroomBreaks[(index + livingroomBreaks.length) % livingroomBreaks.length];
  metricLivingroomBreak.set(lvBreakStatus);
  // ---> Bed Room
  var brBreakStatus = bedroomBreaks[(index + bedroomBreaks.length) % bedroomBreaks.length];
  metricbedroomBreak.set(brBreakStatus);
  // ---> Garden
  var gBreakStatus = gardenBreaks[(index + gardenBreaks.length) % gardenBreaks.length];
  metricGardenBreak.set(gBreakStatus);
  // ---> Active Devices
  numberOfActiveDevices.set(3);
  // ---> Defective Devices
  numberOfDefectiveDevices.set(1);
  // ---> Is Safe
  var isSafe = lvBreakStatus + brBreakStatus + gBreakStatus + isBreak;
  metricIsSafe.set(isSafe);
  console.log('metricIsSafe :' + isSafe + ', index: ' + index)
  // ---> Total Burgleries
  if ((burglaryLastStatus == 0) && isSafe > 0) {
    numOfTotalBurglaries.inc();
  }
  // ---> Total Actual Breaks
  if ((breakLastStatus == 0) && isBreak > 0) {
    numOfBreaks.inc();
  }

  // Retrieve route from request object
  const route = url.parse(req.url).pathname

  if (route === '/metrics') {
    // Return all metrics the Prometheus exposition format
    res.setHeader('Content-Type', register.contentType)
    res.end(await register.metrics())
  }

  burglaryLastStatus = isSafe;
  breakLastStatus = isBreak;
  index++;
})

// Start the HTTP server which exposes the metrics on http://localhost:8080/metrics
server.listen(8080)
console.log('Listening...')