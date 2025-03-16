// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vcpu__Syms.h"


VL_ATTR_COLD void Vcpu___024root__trace_init_sub__TOP__0(Vcpu___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vcpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcpu___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+46,"clk", false,-1);
    tracep->declBit(c+47,"rst", false,-1);
    tracep->pushNamePrefix("cpu ");
    tracep->declBit(c+46,"clk", false,-1);
    tracep->declBit(c+47,"rst", false,-1);
    tracep->declBus(c+5,"pc", false,-1, 31,0);
    tracep->declBus(c+6,"instruction", false,-1, 31,0);
    tracep->declBus(c+7,"rd", false,-1, 4,0);
    tracep->declBus(c+8,"rs1", false,-1, 4,0);
    tracep->declBus(c+9,"rs2", false,-1, 4,0);
    tracep->declBus(c+10,"imm", false,-1, 31,0);
    tracep->declBit(c+11,"inst_type", false,-1);
    tracep->declBus(c+12,"rs1_value", false,-1, 31,0);
    tracep->declBus(c+13,"alu_result", false,-1, 31,0);
    tracep->pushNamePrefix("IDU ");
    tracep->declBus(c+48,"DATA_WIDTH", false,-1, 31,0);
    tracep->declBit(c+46,"clk", false,-1);
    tracep->declBus(c+6,"inst", false,-1, 31,0);
    tracep->declBus(c+7,"rd", false,-1, 4,0);
    tracep->declBus(c+8,"rs1", false,-1, 4,0);
    tracep->declBus(c+9,"rs2", false,-1, 4,0);
    tracep->declBus(c+10,"imm", false,-1, 31,0);
    tracep->declBit(c+11,"inst_type", false,-1);
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("IFU ");
    tracep->declBus(c+48,"DATA_WIDTH", false,-1, 31,0);
    tracep->declBit(c+46,"clk", false,-1);
    tracep->declBit(c+47,"rst", false,-1);
    tracep->declBus(c+5,"pc_now", false,-1, 31,0);
    tracep->declBus(c+5,"pc_next", false,-1, 31,0);
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("imem ");
    tracep->declBus(c+5,"addr", false,-1, 31,0);
    tracep->declBus(c+6,"data", false,-1, 31,0);
    for (int i = 0; i < 4; ++i) {
        tracep->declBus(c+1+i*1,"mem", true,(i+0), 31,0);
    }
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("regdata ");
    tracep->declBus(c+49,"ADDR_WIDTH", false,-1, 31,0);
    tracep->declBus(c+48,"DATA_WIDTH", false,-1, 31,0);
    tracep->declBit(c+46,"clk", false,-1);
    tracep->declBus(c+13,"wdata", false,-1, 31,0);
    tracep->declBus(c+7,"waddr", false,-1, 4,0);
    tracep->declBit(c+11,"wen", false,-1);
    tracep->declBus(c+8,"raddr1", false,-1, 4,0);
    tracep->declBus(c+12,"rdata1", false,-1, 31,0);
    for (int i = 0; i < 32; ++i) {
        tracep->declBus(c+14+i*1,"rf", true,(i+0), 31,0);
    }
    tracep->popNamePrefix(2);
}

VL_ATTR_COLD void Vcpu___024root__trace_init_top(Vcpu___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vcpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcpu___024root__trace_init_top\n"); );
    // Body
    Vcpu___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vcpu___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vcpu___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vcpu___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vcpu___024root__trace_register(Vcpu___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vcpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcpu___024root__trace_register\n"); );
    // Body
    tracep->addFullCb(&Vcpu___024root__trace_full_top_0, vlSelf);
    tracep->addChgCb(&Vcpu___024root__trace_chg_top_0, vlSelf);
    tracep->addCleanupCb(&Vcpu___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vcpu___024root__trace_full_sub_0(Vcpu___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vcpu___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcpu___024root__trace_full_top_0\n"); );
    // Init
    Vcpu___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vcpu___024root*>(voidSelf);
    Vcpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vcpu___024root__trace_full_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vcpu___024root__trace_full_sub_0(Vcpu___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vcpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcpu___024root__trace_full_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+1,(vlSelf->cpu__DOT__imem__DOT__mem[0]),32);
    bufp->fullIData(oldp+2,(vlSelf->cpu__DOT__imem__DOT__mem[1]),32);
    bufp->fullIData(oldp+3,(vlSelf->cpu__DOT__imem__DOT__mem[2]),32);
    bufp->fullIData(oldp+4,(vlSelf->cpu__DOT__imem__DOT__mem[3]),32);
    bufp->fullIData(oldp+5,(vlSelf->cpu__DOT__pc),32);
    bufp->fullIData(oldp+6,(vlSelf->cpu__DOT__instruction),32);
    bufp->fullCData(oldp+7,((0x1fU & (vlSelf->cpu__DOT__instruction 
                                      >> 7U))),5);
    bufp->fullCData(oldp+8,((0x1fU & (vlSelf->cpu__DOT__instruction 
                                      >> 0xfU))),5);
    bufp->fullCData(oldp+9,((0x1fU & (vlSelf->cpu__DOT__instruction 
                                      >> 0x14U))),5);
    bufp->fullIData(oldp+10,((((- (IData)((vlSelf->cpu__DOT__instruction 
                                           >> 0x1fU))) 
                               << 0xcU) | (vlSelf->cpu__DOT__instruction 
                                           >> 0x14U))),32);
    bufp->fullBit(oldp+11,((0x13U == (0x7fU & vlSelf->cpu__DOT__instruction))));
    bufp->fullIData(oldp+12,(((0U == (0x1fU & (vlSelf->cpu__DOT__instruction 
                                               >> 0xfU)))
                               ? 0U : vlSelf->cpu__DOT__regdata__DOT__rf
                              [(0x1fU & (vlSelf->cpu__DOT__instruction 
                                         >> 0xfU))])),32);
    bufp->fullIData(oldp+13,(((((- (IData)((vlSelf->cpu__DOT__instruction 
                                            >> 0x1fU))) 
                                << 0xcU) | (vlSelf->cpu__DOT__instruction 
                                            >> 0x14U)) 
                              + ((0U == (0x1fU & (vlSelf->cpu__DOT__instruction 
                                                  >> 0xfU)))
                                  ? 0U : vlSelf->cpu__DOT__regdata__DOT__rf
                                 [(0x1fU & (vlSelf->cpu__DOT__instruction 
                                            >> 0xfU))]))),32);
    bufp->fullIData(oldp+14,(vlSelf->cpu__DOT__regdata__DOT__rf[0]),32);
    bufp->fullIData(oldp+15,(vlSelf->cpu__DOT__regdata__DOT__rf[1]),32);
    bufp->fullIData(oldp+16,(vlSelf->cpu__DOT__regdata__DOT__rf[2]),32);
    bufp->fullIData(oldp+17,(vlSelf->cpu__DOT__regdata__DOT__rf[3]),32);
    bufp->fullIData(oldp+18,(vlSelf->cpu__DOT__regdata__DOT__rf[4]),32);
    bufp->fullIData(oldp+19,(vlSelf->cpu__DOT__regdata__DOT__rf[5]),32);
    bufp->fullIData(oldp+20,(vlSelf->cpu__DOT__regdata__DOT__rf[6]),32);
    bufp->fullIData(oldp+21,(vlSelf->cpu__DOT__regdata__DOT__rf[7]),32);
    bufp->fullIData(oldp+22,(vlSelf->cpu__DOT__regdata__DOT__rf[8]),32);
    bufp->fullIData(oldp+23,(vlSelf->cpu__DOT__regdata__DOT__rf[9]),32);
    bufp->fullIData(oldp+24,(vlSelf->cpu__DOT__regdata__DOT__rf[10]),32);
    bufp->fullIData(oldp+25,(vlSelf->cpu__DOT__regdata__DOT__rf[11]),32);
    bufp->fullIData(oldp+26,(vlSelf->cpu__DOT__regdata__DOT__rf[12]),32);
    bufp->fullIData(oldp+27,(vlSelf->cpu__DOT__regdata__DOT__rf[13]),32);
    bufp->fullIData(oldp+28,(vlSelf->cpu__DOT__regdata__DOT__rf[14]),32);
    bufp->fullIData(oldp+29,(vlSelf->cpu__DOT__regdata__DOT__rf[15]),32);
    bufp->fullIData(oldp+30,(vlSelf->cpu__DOT__regdata__DOT__rf[16]),32);
    bufp->fullIData(oldp+31,(vlSelf->cpu__DOT__regdata__DOT__rf[17]),32);
    bufp->fullIData(oldp+32,(vlSelf->cpu__DOT__regdata__DOT__rf[18]),32);
    bufp->fullIData(oldp+33,(vlSelf->cpu__DOT__regdata__DOT__rf[19]),32);
    bufp->fullIData(oldp+34,(vlSelf->cpu__DOT__regdata__DOT__rf[20]),32);
    bufp->fullIData(oldp+35,(vlSelf->cpu__DOT__regdata__DOT__rf[21]),32);
    bufp->fullIData(oldp+36,(vlSelf->cpu__DOT__regdata__DOT__rf[22]),32);
    bufp->fullIData(oldp+37,(vlSelf->cpu__DOT__regdata__DOT__rf[23]),32);
    bufp->fullIData(oldp+38,(vlSelf->cpu__DOT__regdata__DOT__rf[24]),32);
    bufp->fullIData(oldp+39,(vlSelf->cpu__DOT__regdata__DOT__rf[25]),32);
    bufp->fullIData(oldp+40,(vlSelf->cpu__DOT__regdata__DOT__rf[26]),32);
    bufp->fullIData(oldp+41,(vlSelf->cpu__DOT__regdata__DOT__rf[27]),32);
    bufp->fullIData(oldp+42,(vlSelf->cpu__DOT__regdata__DOT__rf[28]),32);
    bufp->fullIData(oldp+43,(vlSelf->cpu__DOT__regdata__DOT__rf[29]),32);
    bufp->fullIData(oldp+44,(vlSelf->cpu__DOT__regdata__DOT__rf[30]),32);
    bufp->fullIData(oldp+45,(vlSelf->cpu__DOT__regdata__DOT__rf[31]),32);
    bufp->fullBit(oldp+46,(vlSelf->clk));
    bufp->fullBit(oldp+47,(vlSelf->rst));
    bufp->fullIData(oldp+48,(0x20U),32);
    bufp->fullIData(oldp+49,(5U),32);
}
