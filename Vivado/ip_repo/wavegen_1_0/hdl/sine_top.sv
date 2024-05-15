`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/28/2023 06:35:39 PM
// Design Name: 
// Module Name: sine_top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module sine_top(
    input clk,
    input en,
    input freq_A,
    input freq_B,
    output [11:0] phase_A   ,
    output [11:0] phase_B   ,
    input  [15:0] sine_A   ,
    input  [15:0] sine_B   

    );
endmodule
