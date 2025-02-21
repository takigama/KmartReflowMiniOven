// small chunk of nodejs code to do the following:
// 1. set duty to x %, wait for stable tempurature, let cool
// 2. set duty to x+10%, wait for stable tempurature, let cool
// ... etc then store the temp profile ramping speed - this'll be total fun
const request = require('sync-request');

//console.log("TRY TWO: ",parseFloat(String(mytemp.body)));

var myip = "";
var keeplooping = true;
var heatstarted = false;
var currentDuty = 0;
var coolingDown = true;
const coolWhen = 30; // temperature we consider "cool"
var dutyEnd = 70;
var lastTempTime = 0;
var ctemp = 0;
var lastTemp;
var tdICare = 5;

function getTemp() {
    var mytemp = request('GET', 'http://' + myip + '/gettemp1');
    let thistemp = parseFloat(String(mytemp.body));
    // console.log("Temp: ",thistemp);
    return thistemp;
}

function setDuty(duty) {
    var mytemp = request('GET', 'http://' + myip + '/setduty?duty=' + duty);
    return parseFloat(String(mytemp.body));
}

function getsecs() {
    var d = new Date();
    var seconds = Math.round(d.getTime() / 1000);
    return seconds;
}

function sleep(ms) {
    return new Promise((resolve) => {
        setTimeout(resolve, ms);
    });
}

async function jogmain() {

    while (keeplooping) {
        ctemp = getTemp();
        if (coolingDown) console.log("Cooling")
        else if (heatstarted) console.log("Heating")
        else console.log("Not heating or cooling... odd?");
        console.log("TL," + getsecs() + "," + currentDuty + "," + ctemp)
        // ok... this be fun?
        if (!coolingDown && !heatstarted) {

            if (ctemp > coolWhen) {
                // we wait
                console.log("cooling down still");
                setDuty(0);
            } else {
                // add to duty and send
                currentDuty += 10;
                setDuty(currentDuty);
                heatstarted = true;
                lastTempTime = getsecs();
                console.log("Starting temp with duty of " + currentDuty);
            }
        } else if (heatstarted) {
            ctemp = getTemp();
            console.log(getsecs() + "," + ctemp);
            if ((ctemp - lastTemp) > tdICare) {
                lastTempTime = getsecs();
                lastTemp = ctemp;
            } else if ((getsecs() - lastTempTime) > 300) {
                // we've been 5 minutes without a significant temperature change, turn it off
                console.log("Temperature was stable - cooldown now");
                heatstarted = false;
                coolingDown = true;
                setDuty(0);
            }
        } else if (coolingDown) {
            ctemp = getTemp();
            setDuty(0);
            if (ctemp < coolWhen) {
                coolingDown = false;
                lastTempTime = getsecs();
                console.log("I am cool now");
                if (currentDuty >= 70) {
                    console.log("at or past end duty");
                    keeplooping = false;

                }
            }
        }
        await sleep(1000);
    }
}

if (typeof process.argv[2] == "undefined") {
    console.log("Usage: command ipaddress");
    process.exit(0);
}
myip = process.argv[2];
console.log("myip: ", myip);
jogmain();