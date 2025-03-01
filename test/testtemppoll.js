// small chunk of nodejs code to do the following:
// 1. set duty to x %, wait for stable tempurature, let cool
// 2. set duty to x+10%, wait for stable tempurature, let cool
// ... etc then store the temp profile ramping speed - this'll be total fun
// i really need to re-write this like its a state engine, but its not too
// important as its really a bit of one-time test code.
// npm install sync-request before you use it
const request = require('sync-request');
const fs = require('fs');


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
var lastTemp = 0;
var tdICare = 2;
var fd;

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
        fs.appendFileSync("data.cduty."+currentDuty, "TL," + getsecs() + "," + currentDuty + "," + ctemp+"\n")
        
        // ok... this be fun?
        if (!coolingDown && !heatstarted) {
            console.log("in first loop section");
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
            console.log("In heatstarted loop");
            ctemp = getTemp();
            console.log(getsecs() + "," + ctemp);
            console.log("ctemp ",ctemp, " lasttemp ",lastTemp);
            if ((ctemp - lastTemp) > tdICare) {
                lastTempTime = getsecs();
                console.log("LastTempTime now ",lastTempTime);
                lastTemp = ctemp;
            } else if ((getsecs() - lastTempTime) > 300) {
                // we've been 5 minutes without a significant temperature change, turn it off
                console.log("Temperature was stable - cooldown now");
                heatstarted = false;
                coolingDown = true;
                setDuty(0);
            }
        } else if (coolingDown) {
            console.log("in coolingdown section");
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
        console.log("End loop\n\n\n\n");
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