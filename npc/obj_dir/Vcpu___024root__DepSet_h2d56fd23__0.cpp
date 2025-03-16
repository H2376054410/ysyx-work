// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcpu.h for the primary calling header

#include "verilated.h"

#include "Vcpu___024root.h"

void Vcpu___024root___eval_act(Vcpu___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcpu___024root___eval_act\n"); );
}

VL_INLINE_OPT void Vcpu___024root___nba_sequent__TOP__0(Vcpu___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcpu___024root___nba_sequent__TOP__0\n"); );
    // Init
    CData/*4:0*/ __Vdlyvdim0__cpu__DOT__regdata__DOT__rf__v0;
    __Vdlyvdim0__cpu__DOT__regdata__DOT__rf__v0 = 0;
    IData/*31:0*/ __Vdlyvval__cpu__DOT__regdata__DOT__rf__v0;
    __Vdlyvval__cpu__DOT__regdata__DOT__rf__v0 = 0;
    CData/*0:0*/ __Vdlyvset__cpu__DOT__regdata__DOT__rf__v0;
    __Vdlyvset__cpu__DOT__regdata__DOT__rf__v0 = 0;
    // Body
    VL_WRITEF("the value of x1 is %10#\nthe value of inst is %x\n",
              32,vlSelf->cpu__DOT__regdata__DOT__rf
              [1U],32,vlSelf->cpu__DOT__instruction);
    __Vdlyvset__cpu__DOT__regdata__DOT__rf__v0 = 0U;
    vlSelf->cpu__DOT__pc = ((IData)(vlSelf->rst) ? 0x80000000U
                             : ((IData)(4U) + vlSelf->cpu__DOT__pc));
    if (((0x13U == (0x7fU & vlSelf->cpu__DOT__instruction)) 
         & (0U != (0x1fU & (vlSelf->cpu__DOT__instruction 
                            >> 7U))))) {
        __Vdlyvval__cpu__DOT__regdata__DOT__rf__v0 
            = ((((- (IData)((vlSelf->cpu__DOT__instruction 
                             >> 0x1fU))) << 0xcU) | 
                (vlSelf->cpu__DOT__instruction >> 0x14U)) 
               + vlSelf->cpu__DOT__rs1_value);
        __Vdlyvset__cpu__DOT__regdata__DOT__rf__v0 = 1U;
        __Vdlyvdim0__cpu__DOT__regdata__DOT__rf__v0 
            = (0x1fU & (vlSelf->cpu__DOT__instruction 
                        >> 7U));
    }
    if (__Vdlyvset__cpu__DOT__regdata__DOT__rf__v0) {
        vlSelf->cpu__DOT__regdata__DOT__rf[__Vdlyvdim0__cpu__DOT__regdata__DOT__rf__v0] 
            = __Vdlyvval__cpu__DOT__regdata__DOT__rf__v0;
    }
    vlSelf->cpu__DOT__instruction = vlSelf->cpu__DOT__imem__DOT__mem
        [(3U & (vlSelf->cpu__DOT__pc >> 2U))];
    vlSelf->cpu__DOT__rs1_value = ((0U == (0x1fU & 
                                           (vlSelf->cpu__DOT__instruction 
                                            >> 0xfU)))
                                    ? 0U : vlSelf->cpu__DOT__regdata__DOT__rf
                                   [(0x1fU & (vlSelf->cpu__DOT__instruction 
                                              >> 0xfU))]);
}

void Vcpu___024root___eval_nba(Vcpu___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcpu___024root___eval_nba\n"); );
    // Body
    if (vlSelf->__VnbaTriggered.at(0U)) {
        Vcpu___024root___nba_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[1U] = 1U;
    }
}

void Vcpu___024root___eval_triggers__act(Vcpu___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vcpu___024root___dump_triggers__act(Vcpu___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vcpu___024root___dump_triggers__nba(Vcpu___024root* vlSelf);
#endif  // VL_DEBUG

void Vcpu___024root___eval(Vcpu___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcpu___024root___eval\n"); );
    // Init
    VlTriggerVec<1> __VpreTriggered;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        __VnbaContinue = 0U;
        vlSelf->__VnbaTriggered.clear();
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            vlSelf->__VactContinue = 0U;
            Vcpu___024root___eval_triggers__act(vlSelf);
            if (vlSelf->__VactTriggered.any()) {
                vlSelf->__VactContinue = 1U;
                if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                    Vcpu___024root___dump_triggers__act(vlSelf);
#endif
                    VL_FATAL_MT("vsrc/cpu.v", 75, "", "Active region did not converge.");
                }
                vlSelf->__VactIterCount = ((IData)(1U) 
                                           + vlSelf->__VactIterCount);
                __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
                vlSelf->__VnbaTriggered.set(vlSelf->__VactTriggered);
                Vcpu___024root___eval_act(vlSelf);
            }
        }
        if (vlSelf->__VnbaTriggered.any()) {
            __VnbaContinue = 1U;
            if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
                Vcpu___024root___dump_triggers__nba(vlSelf);
#endif
                VL_FATAL_MT("vsrc/cpu.v", 75, "", "NBA region did not converge.");
            }
            __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
            Vcpu___024root___eval_nba(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
void Vcpu___024root___eval_debug_assertions(Vcpu___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vcpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcpu___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->rst & 0xfeU))) {
        Verilated::overWidthError("rst");}
}
#endif  // VL_DEBUG
