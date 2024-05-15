// 4356 Lab 6 MCP4822 SPI Engine for Waveform Generator
// Rolando Rosales 1001850424
// Duy Nguyen 1001943563

module spi_engine(
    input CLK100,           // 100 MHz clock input
    output [9:0] LED,       // RGB1, RGB0, LED 9..0 placed from left to right
    output [2:0] RGB0,      
    output [2:0] RGB1,
    output [3:0] SS_ANODE,   // Anodes 3..0 placed from left to right
    output [7:0] SS_CATHODE, // Bit order: DP, G, F, E, D, C, B, A
    input [11:0] SW,         // SWs 11..0 placed from left to right
    input [3:0] PB,          // PBs 3..0 placed from left to right
    inout [23:0] GPIO,       // PMODA-C 1P, 1N, ... 3P, 3N order
    output [3:0] SERVO,      // Servo outputs
    output PDM_SPEAKER,      // PDM signals for mic and speaker
    input PDM_MIC_DATA,      
    output PDM_MIC_CLK,
    output ESP32_UART1_TXD,  // WiFi/Bluetooth serial interface 1
    input ESP32_UART1_RXD,
    output IMU_SCLK,         // IMU spi clk
    output IMU_SDI,          // IMU spi data input
    input IMU_SDO_AG,        // IMU spi data output (accel/gyro)
    input IMU_SDO_M,         // IMU spi data output (mag)
    output IMU_CS_AG,        // IMU cs (accel/gyro) 
    output IMU_CS_M,         // IMU cs (mag)
    input IMU_DRDY_M,        // IMU data ready (mag)
    input IMU_INT1_AG,       // IMU interrupt (accel/gyro)
    input IMU_INT_M,         // IMU interrupt (mag)
    output IMU_DEN_AG        // IMU data enable (accel/gyro)
    );
        
    // Terminate all of the unused outputs or i/o's
    assign LED = 10'b0000000000;
    assign RGB0 = 3'b000;
    assign RGB1 = 3'b000;
    assign SS_ANODE = 4'b1111;
    assign SS_CATHODE = 8'b11111111;
    assign GPIO [7:0] = 8'bzzzzzzzz;
    assign GPIO [15:12] = 4'bzzzz;
    assign GPIO [23:20] = 4'bzzzz;
    assign SERVO = 4'b0000;
    assign PDM_SPEAKER = 1'b0;
    assign PDM_MIC_CLK = 1'b0;
    assign ESP32_UART1_TXD = 1'b0;
    assign IMU_SCLK = 1'b0;
    assign IMU_SDI = 1'b0;
    assign IMU_CS_AG = 1'b1;
    assign IMU_CS_M = 1'b1;
    assign IMU_DEN_AG = 1'b0;
    
    reg clk4m;                  // 4 MHz clock
    reg clk4m_del;                  // 4 MHz clock
    wire pulse_4m;                  // 4 MHz clock
    reg SDI;                    // SDI Signal 
    reg SCK;                    // SCK Signal
    reg CSn;                    // CS active-low signal
    reg LDACn;                  // LDAC active-low signal
    reg [2:0] state;            // Used to represent the current state
    reg [4:0] count;            // Counter used within each state
    reg [5:0] div4;             // Used to divide clock to 4 MHz
    reg [15:0] SDI_DATA_A;
    reg [15:0] SDI_DATA_B;
    wire signed [11:0] out_a;
    wire signed [11:0] out_b;
    
    reg [5:0] ofs_a;// = 6'd16;
    reg [5:0] ofs_b;// = 6'd40;
    
    wire PL_clk_100MHz;

    assign GPIO[16] = SDI;      // assign SDI Signal
    assign GPIO[17] = SCK;      // assign SCK Signal
    assign GPIO[18] = CSn;      // assign CS active-low signal
    assign GPIO[19] = LDACn;    // assign LDAC active-low signal
    
    assign GPIO[8] = SDI;       // assign SDI Signal for Logic Analyzer
    assign GPIO[9] = SCK;       // assign SCK Signal for Logic Analyzer
    assign GPIO[10] = CSn;      // assign CS active-low signal for Logic Analyzer 
    assign GPIO[11] = LDACn;    // assign LDAC active-low signal for Logic Analyzer
    
    assign ofs_a = SW[5:0]; 
    assign ofs_b = SW[11:6];
    
    initial
    begin
        CSn = 1;
        LDACn = 1;
        state = 0;
    end
    
    always_ff @(posedge PL_clk_100MHz)
    begin
       clk4m_del <= clk4m   ;
    end 
    
    assign pulse_4m = clk4m & ~clk4m_del    ;
    
    //always_ff @ (posedge clk4m) // signed 2 unsigned
    always_ff @ (posedge PL_clk_100MHz) // signed 2 unsigned
    begin
    if(pulse_4m)
    begin
        if (out_a[11] == 1'b0) SDI_DATA_A[11:0] <= ((~out_a[10:0] - 12'h800) + 1'b1) + ofs_a;
        if (out_b[11] == 1'b0) SDI_DATA_B[11:0] <= ((~out_b[10:0] - 12'h800) + 1'b1) + ofs_b;
        if (out_a[11] == 1'b1) SDI_DATA_A[11:0] <= (~out_a[10:0] + 1'b1) + ofs_a;
        if (out_b[11] == 1'b1) SDI_DATA_B[11:0] <= (~out_b[10:0] + 1'b1) + ofs_b;
        SDI_DATA_A [15:12] <= 4'b0011;
        SDI_DATA_B [15:12] <= 4'b1011;
        //SDI_DATA_A  <= {4'b0011,12'hBBB};
        //SDI_DATA_B  <= {4'b1011,12'h555};
    end 
    end
    
    /*
ila_0 ila_inst (
	.clk(PL_clk_100MHz), // input wire clk
	.probe0(SDI_DATA_A), // input wire [11:0]  probe0  
	.probe1(SDI_DATA_B), // input wire [11:0]  probe1 
	.probe2(clk4m), // input wire [0:0]  probe2
	.probe3(SDI), // input wire [0:0]  probe2
	.probe4(SCK), // input wire [0:0]  probe2
	.probe5(CSn), // input wire [0:0]  probe2
	.probe6(LDACn) // input wire [0:0]  probe2
);
*/

    
    // 4 MHz clock divider
    always_ff @(posedge PL_clk_100MHz)
    begin
        div4 <= div4 + 1'b1;
        if (div4 >= 5)
        begin
            div4 <= 5'd0;
            clk4m <= ~clk4m;
        end
    end
    
    //always_ff @(posedge clk4m)
    //begin
    always_ff @ (posedge PL_clk_100MHz) // signed 2 unsigned
    begin
    if(pulse_4m)
    begin
        case (state)
            3'b000:
            begin
                CSn <= 0;
                LDACn <= 1;
                SDI <= SDI_DATA_A[15];
                
                count <= count + 1;
                
                if (count == 2)
                begin
                    state <= 3'b001;
                    count <= 0;
                end
            end
            3'b001:
            begin
                CSn <= 0;
                LDACn <= 1;
                
                if (SCK)
                    begin
                        SDI <= SDI_DATA_A[14 - count];
                            
                        count <= count + 1;
                                                   
                        if (count == 15)
                        begin
                            state <= 3'b010;
                            count <= 0;
                        end
                        
                        SCK <= 0;
                    end
                    else SCK <= 1;
            end
            3'b010:
            begin
                CSn <= 1;
                LDACn <= 1;
                
                count <= count + 1;
                
                if (count == 4)
                begin
                    state <= 3'b011;
                    count <= 0;
                end
            end
            3'b011:
            begin
                CSn <= 0;
                LDACn <= 1;
                SDI <= SDI_DATA_B[15];
                
                count <= count + 1;
                
                if (count == 2)
                begin
                    state <= 3'b100;
                    count <= 0;
                end
            end
            3'b100:
            begin
                CSn <= 0;
                LDACn <= 1;
                
                if (SCK)
                    begin
                        SDI <= SDI_DATA_B[14 - count];
                            
                        count <= count + 1;
                                                   
                        if (count == 15)
                        begin
                            state <= 3'b101;
                            count <= 0;
                        end
                        
                        SCK <= 0;
                    end
                    else SCK <= 1;
            end
            3'b101:
            begin
                count <= count + 1;
                
                if (count == 2)
                begin
                    CSn <= 1;
                    LDACn <= 1;
                end
                if (count == 4)
                begin
                    CSn <= 1;
                    LDACn <= 0;
                end
                if (count == 8)
                begin
                    CSn <= 1;
                    LDACn <= 1;
                end
                if (count == 10)
                begin
                    state <= 3'b000;
                    count <= 0;
                end
            end
        endcase
    end
    end
    
    // Instantiate system wrapper
    system_wrapper system_ins (
        .DDR_addr(DDR_addr),
        .DDR_ba(DDR_ba),
        .DDR_cas_n(DDR_cas_n),
        .DDR_ck_n(DDR_ck_n),
        .DDR_ck_p(DDR_ck_p),
        .DDR_cke(DDR_cke),
        .DDR_cs_n(DDR_cs_n),
        .DDR_dm(DDR_dm),
        .DDR_dq(DDR_dq),
        .DDR_dqs_n(DDR_dqs_n),
        .DDR_dqs_p(DDR_dqs_p),
        .DDR_odt(DDR_odt),
        .DDR_ras_n(DDR_ras_n),
        .DDR_reset_n(DDR_reset_n),
        .DDR_we_n(DDR_we_n),
        .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
        .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
        .FIXED_IO_mio(FIXED_IO_mio),
        .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
        .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
        .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),
        .out_a(out_a),
        .out_b(out_b),
        .PL_clk_100MHz(PL_clk_100MHz),
        .LDACn(LDACn)
        );

endmodule