var prox1=0;
var prox2=0;
var prox3=0;
var temp1=0;
var temp2=0;
var temp3=0;
var emer = 6;
var setTemp=165;
var total;
    
    firebase.database().ref("NodeMCU").on('value',function(data){
// console.log(data.val())
var newdata=data.val();
// console.log(newdata) 
    prox1=newdata.slice(0,1)
    prox2=newdata.slice(2,3)
    prox3=newdata.slice(4,5)
    temp1=newdata.slice(6,8)
    temp2=newdata.slice(9,11)
    temp3=newdata.slice(12,14)
// console.log(temp2) 


var prsen1 = document.getElementById("prsen1").innerHTML = prox1;
var prsen2 = document.getElementById("prsen2").innerHTML = prox2;

var prsen3 = document.getElementById("prsen3").innerHTML = prox3;

var temp1 = document.getElementById("temp1").innerHTML = temp1;
var temp2 = document.getElementById("temp2").innerHTML = temp2;


var temp3 = document.getElementById("temp3").innerHTML = temp3;


var a = parseInt(temp1)
var b = parseInt(temp2)

var c = parseInt(temp3)


var average = (a+b+c)/3;

 document.getElementById("average").innerHTML = average.toFixed(0);












    })



    function emergency(val){

        emer = val;
        console.log(emer);
    //     var value1 ={
    // emergency : emer
        total =`${emer},${setTemp}`
    firebase.database().ref('emergency').set(total)

    // var btn =document.getElementById('emergency').setAttribute("onclick", "emergency(6)");
    // console.log(btn)
    
    if(val == 6){
    var btn =document.getElementById('emergency');
    btn.setAttribute("onclick", "emergency(5)");
    btn.classList.remove("button1");      
    }
    else if(val == 5){
        var btn =document.getElementById('emergency');
        btn.setAttribute("onclick", "emergency(6)");
        btn.classList.add("button1");                
        }
}

// $(document).ready(function(){
//     $("a").toggle(
//         $(this).click(function(){$(this).attr('onclick', 'images/image2.jpg');}),
//         function(){$(this).attr('onclick', 'images/image3.jpg');
//      });
//   });

// {

//     }


function settemp(){

var input = +prompt("what temperature you want to set")
setTemp = input+100;

total =`${emer},${setTemp}`
firebase.database().ref('emergency').set(total)

document.getElementById('setTemp').innerHTML=input;




}