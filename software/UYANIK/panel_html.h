// Copyright (C) 2026 rutubetsiz
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <Arduino.h>

/*
  Bu dosya üretiliyor. Elle düzenleme yapmayın.
  Kaynak: panel.html
  Üretmek için: python build_html.py
  Kontrol etmek için: python build_html.py --kontrol

  This file is generated. Do not edit it manually.
  Source: panel.html
  To generate: python build_html.py
  To check: python build_html.py --kontrol
*/

const char PANEL_HTML[] PROGMEM = R"HTMLPAGE(<!doctype html>
<!--
  Copyright (C) 2026 rutubetsiz
  SPDX-License-Identifier: GPL-3.0-or-later
-->
<html lang="tr">
  <head>
    <meta charset="utf-8" />
    <meta
      name="viewport"
      content="width=device-width,initial-scale=1,viewport-fit=cover"
    />
    <meta name="color-scheme" content="light dark" />
    <title>UYANIK</title>
    <style>
      *,
      *::before,
      *::after {
        box-sizing: border-box;
        margin: 0;
        padding: 0;
      }

      [hidden] {
        display: none !important;
      }

      :root {
        --bg: #f1f1f3;
        --card: #ffffff;
        --track: #e7e7eb;
        --soft: #f3f3f6;
        --ink: #16181c;
        --dim: #8a9099;
        --sel: #e7e7eb;
        --sel-ink: #16181c;
        --red: #ff3b30;
        --red-soft: #ffebe9;
        --amber: #ffb020;
        --amber-soft: #fff3dc;
        --green: #30c36b;
        --green-ink: #1b9e55;
        --line: #e3e3e8;
        --btn: #1c1f24;
        --btn-ink: #ffffff;
      }

      @media (prefers-color-scheme: dark) {
        :root {
          --bg: #0e0f11;
          --card: #17191c;
          --track: #2a2e34;
          --soft: #212429;
          --ink: #f2f3f5;
          --dim: #8b9199;
          --sel: #2a2e34;
          --sel-ink: #f2f3f5;
          --red: #ff6b5e;
          --red-soft: rgba(255, 107, 94, 0.15);
          --amber: #ffbc42;
          --amber-soft: rgba(255, 188, 66, 0.15);
          --green: #3ad47c;
          --green-ink: #57dd93;
          --line: #292d33;
          --btn: #f2f3f5;
          --btn-ink: #16181c;
        }
      }

      body {
        background: var(--bg);
        color: var(--ink);
        min-height: 100dvh;
        font:
          400 16px/1.45 system-ui,
          -apple-system,
          "Segoe UI",
          Roboto,
          "Helvetica Neue",
          Arial,
          sans-serif;
        -webkit-font-smoothing: antialiased;
        -webkit-tap-highlight-color: transparent;
      }

      button {
        font: inherit;
        color: inherit;
        background: none;
        border: none;
        cursor: pointer;
      }

      :focus-visible {
        outline: 2.5px solid var(--ink);
        outline-offset: 3px;
      }

      svg {
        display: block;
        fill: currentColor;
      }

      /* LOADING */
      .boot {
        min-height: 100dvh;
        display: grid;
        place-items: center;
      }

      .spin {
        width: 28px;
        height: 28px;
        border-radius: 50%;
        border: 5px solid var(--track);
        border-top-color: var(--ink);
        animation: spin 1s linear infinite;
      }

      @keyframes spin {
        to {
          transform: rotate(360deg);
        }
      }

      /* LOGIN SCREEN */
      .gate {
        min-height: 100dvh;
        display: flex;
        align-items: center;
        justify-content: center;
        padding: 24px;
      }

      .gcard {
        position: relative;
        width: 100%;
        max-width: 392px;
        background: var(--card);
        border: 1px solid var(--line);
        border-radius: 26px;
        padding: 30px 22px 22px;
      }

      .gcard > .lang {
        position: absolute;
        top: 16px;
        right: 16px;
      }

      .ghead {
        text-align: center;
        margin-bottom: 22px;
      }

      .ghead h1 {
        font-size: 27px;
        font-weight: 700;
        letter-spacing: -0.03em;
      }

      .ghead p {
        color: var(--dim);
        font-size: 14.5px;
        margin-top: 6px;
      }

      .chk {
        display: flex;
        align-items: center;
        gap: 11px;
        margin-top: 15px;
        color: var(--dim);
        font-size: 14.5px;
        cursor: pointer;
      }

      .chk input {
        width: 19px;
        height: 19px;
        flex: none;
        accent-color: var(--ink);
      }

      .ghint {
        font-size: 13px;
        color: var(--dim);
        line-height: 1.5;
        margin-top: 18px;
        padding-top: 16px;
        border-top: 1px solid var(--line);
      }

      /* APP SHELL */
      .shell {
        display: flex;
        min-height: 100dvh;
      }

      .main {
        flex: 1;
        min-width: 0;
      }

      .app {
        width: 100%;
        max-width: 960px;
        margin: 0 auto;
        padding: 18px 16px calc(102px + env(safe-area-inset-bottom));
      }

      .rail {
        position: fixed;
        left: 0;
        right: 0;
        bottom: 0;
        z-index: 8;
        display: flex;
        gap: 4px;
        background: var(--card);
        border-top: 1px solid var(--line);
        padding: 7px 10px calc(7px + env(safe-area-inset-bottom));
      }

      .rail .brand {
        display: none;
      }

      .rail button {
        flex: 1;
        display: flex;
        flex-direction: column;
        align-items: center;
        gap: 4px;
        padding: 7px 4px;
        border-radius: 15px;
        color: var(--dim);
        font-size: 11.5px;
        font-weight: 600;
        letter-spacing: -0.005em;
        transition:
          color 0.2s,
          background 0.2s;
      }

      .rail button svg {
        width: 23px;
        height: 23px;
      }

      .rail button.on {
        color: var(--ink);
      }

      .mbrand {
        display: flex;
        align-items: center;
        margin-bottom: 16px;
        color: var(--dim);
      }

      .mbrand b {
        font-size: 14px;
        font-weight: 700;
        letter-spacing: 0.06em;
      }

      @media (min-width: 900px) {
        .rail {
          position: sticky;
          top: 0;
          left: auto;
          right: auto;
          bottom: auto;
          align-self: flex-start;
          width: 222px;
          height: 100dvh;
          flex: none;
          flex-direction: column;
          gap: 5px;
          border-top: none;
          border-right: 1px solid var(--line);
          padding: 26px 14px;
        }
        .rail .brand {
          display: flex;
          align-items: center;
          gap: 11px;
          padding: 0 12px;
          margin-bottom: 22px;
          color: var(--ink);
        }
        .rail .brand b {
          font-size: 18px;
          font-weight: 700;
          letter-spacing: 0.04em;
        }
        .rail button {
          flex: none;
          flex-direction: row;
          justify-content: flex-start;
          gap: 13px;
          padding: 11px 12px;
          border-radius: 14px;
          font-size: 15.5px;
          font-weight: 600;
          letter-spacing: -0.012em;
        }
        .rail button svg {
          width: 20px;
          height: 20px;
        }
        .rail button.on {
          background: var(--sel);
        }
        .mbrand {
          display: none;
        }
        .app {
          padding: 38px 32px 56px;
        }
      }

      /* HEADER */

      .top {
        display: flex;
        align-items: flex-start;
        gap: 14px;
        flex-wrap: wrap;
        margin-bottom: 22px;
      }

      h1 {
        font-size: 28px;
        font-weight: 700;
        letter-spacing: -0.032em;
        line-height: 1.1;
      }

      @media (min-width: 900px) {
        h1 {
          font-size: 33px;
        }
      }
      .tools {
        margin-left: auto;
        display: flex;
        align-items: center;
        gap: 9px;
      }

      .live {
        display: flex;
        align-items: center;
        gap: 8px;
        background: var(--card);
        border: 1px solid var(--line);
        border-radius: 999px;
        padding: 7px 13px 7px 11px;
        font-size: 13.5px;
        font-weight: 600;
        white-space: nowrap;
      }

      .live i {
        width: 8px;
        height: 8px;
        border-radius: 50%;
        background: var(--green);
        flex: none;
      }
      .live.off i {
        background: var(--dim);
      }

      /* language selector */
      .iconbtn {
        width: 36px;
        height: 36px;
        border-radius: 999px;
        flex: none;
        display: grid;
        place-items: center;
        background: var(--card);
        border: 1px solid var(--line);
        color: var(--dim);
        transition:
          color 0.2s,
          border-color 0.2s;
      }

      .iconbtn svg {
        width: 17px;
        height: 17px;
      }
      .iconbtn:hover {
        color: var(--red);
        border-color: var(--red);
      }

      .lang {
        display: flex;
        gap: 2px;
        background: var(--track);
        border-radius: 999px;
        padding: 3px;
        flex: none;
      }
      .lang button {
        border-radius: 999px;
        padding: 5px 10px;
        font-size: 12.5px;
        font-weight: 700;
        color: var(--dim);
        letter-spacing: 0.03em;
        transition:
          background 0.2s,
          color 0.2s;
      }
      .lang button.on {
        background: var(--card);
        color: var(--ink);
      }

      h3 {
        font-size: 19px;
        font-weight: 700;
        letter-spacing: -0.022em;
        margin: 26px 0 12px;
      }
      h3:first-child {
        margin-top: 0;
      }

      .cols {
        display: grid;
        gap: 0;
      }

      .cols > div + div {
        margin-top: 32px;
      }
      @media (min-width: 1080px) {
        .cols {
          grid-template-columns: 1.3fr 1fr;
          gap: 26px;
          align-items: start;
        }
        .cols > div + div {
          margin-top: 0;
        }
        .cols > div:last-child h3 {
          margin-top: 0;
        }
      }

      /* DEVICE CARD */
      .device {
        background: var(--card);
        border: 1px solid var(--line);
        border-radius: 26px;
        padding: 18px;
        transition:
          background 0.35s,
          border-color 0.35s;
      }
      .device.on {
        background: var(--green);
        border-color: var(--green);
      }
      .device.down {
        background: var(--card);
      }
      .drow {
        display: flex;
        align-items: center;
        justify-content: space-between;
        margin-bottom: 22px;
      }
      .dico {
        color: var(--dim);
        transition: color 0.3s;
      }
      .dico svg {
        width: 30px;
        height: 30px;
      }
      .device.on .dico {
        color: rgba(255, 255, 255, 0.92);
      }
      .device.down .dico {
        color: var(--red);
      }
      .device h2 {
        font-size: 23px;
        font-weight: 700;
        letter-spacing: -0.028em;
      }
      .device p {
        font-size: 14.5px;
        color: var(--dim);
        margin-top: 5px;
      }
      .device.on h2 {
        color: #fff;
      }
      .device.on p {
        color: rgba(255, 255, 255, 0.8);
      }

      /* TOGGLE SWITCH */
      .sw {
        width: 60px;
        height: 35px;
        border-radius: 999px;
        background: var(--track);
        position: relative;
        flex: none;
        transition: background 0.3s;
      }
      .sw::after {
        content: "";
        position: absolute;
        top: 3px;
        left: 3px;
        width: 29px;
        height: 29px;
        border-radius: 50%;
        background: #fff;
        box-shadow: 0 2px 5px rgba(0, 0, 0, 0.18);
        transition: transform 0.28s cubic-bezier(0.34, 1.4, 0.64, 1);
      }
      .device.on .sw {
        background: rgba(255, 255, 255, 0.34);
      }
      .device.on .sw::after {
        transform: translateX(25px);
      }
      .sw:disabled {
        opacity: 0.55;
      }

      /* COMMANDS */
      .cmds {
        background: var(--card);
        border: 1px solid var(--line);
        border-radius: 20px;
        overflow: hidden;
      }
      .cmd {
        position: relative;
        display: flex;
        align-items: center;
        gap: 14px;
        width: 100%;
        text-align: left;
        padding: 15px 16px;
        transition: background 0.15s;
        -webkit-user-select: none;
        user-select: none;
        -webkit-touch-callout: none;
      }
      .cmd + .cmd {
        border-top: 1px solid var(--line);
      }
      .cmd:active {
        background: var(--soft);
      }
      .cmd:disabled {
        opacity: 0.5;
        cursor: default;
      }
      .cmd > * {
        position: relative;
      }
      .fill {
        position: absolute;
        left: 0;
        top: 0;
        bottom: 0;
        width: 0;
        background: var(--red-soft);
        pointer-events: none;
      }
      .cico {
        flex: none;
        color: var(--ink);
        display: grid;
        place-items: center;
      }
      .cico svg {
        width: 22px;
        height: 22px;
      }
      .cmd.hot .cico {
        color: var(--ink);
      }
      .ctxt {
        flex: 1;
        min-width: 0;
      }
      .ctxt b {
        display: block;
        font-size: 16px;
        font-weight: 600;
        letter-spacing: -0.018em;
      }
      .ctxt i {
        display: block;
        font-style: normal;
        font-size: 13.5px;
        color: var(--dim);
        margin-top: 2px;
      }
      .cmd.hot .ctxt i {
        color: var(--red);
      }
      .cgo {
        flex: none;
        color: var(--dim);
        opacity: 0.5;
      }
      .cgo svg {
        width: 14px;
        height: 14px;
      }

      /* CONNECTION */
      .stats {
        display: grid;
        gap: 10px;
        grid-template-columns: 1fr;
      }
      @media (min-width: 520px) {
        .stats {
          grid-template-columns: 1fr 1fr;
        }
      }
      .stat {
        display: flex;
        align-items: center;
        gap: 13px;
        background: var(--card);
        border: 1px solid var(--line);
        border-radius: 16px;
        padding: 12px 15px;
      }
      .stat > svg {
        width: 20px;
        height: 20px;
        flex: none;
        color: var(--ink);
      }
      .stat div {
        min-width: 0;
      }
      .stat span {
        display: block;
        font-size: 12.5px;
        font-weight: 600;
        color: var(--ink);
      }
      .stat b {
        display: block;
        font-size: 16px;
        font-weight: 600;
        letter-spacing: -0.012em;
        margin-top: 1px;
        font-variant-numeric: tabular-nums;
        overflow: hidden;
        text-overflow: ellipsis;
        white-space: nowrap;
      }

      /* NOTIFICATIONS */
      .toast {
        position: fixed;
        left: 50%;
        bottom: calc(96px + env(safe-area-inset-bottom));
        transform: translate(-50%, 18px) scale(0.96);
        display: flex;
        align-items: center;
        gap: 10px;
        background: var(--green);
        color: #ffffff;
        border: 1px solid transparent;
        border-radius: 999px;
        padding: 12px 20px;
        font-size: 14.5px;
        font-weight: 600;
        white-space: nowrap;
        max-width: calc(100vw - 32px);
        opacity: 0;
        pointer-events: none;
        transition:
          opacity 0.22s,
          transform 0.28s cubic-bezier(0.34, 1.3, 0.64, 1);
        z-index: 9;
      }
      @media (min-width: 900px) {
        .toast {
          bottom: 26px;
        }
      }
      .toast.show {
        opacity: 1;
        transform: translate(-50%, 0) scale(1);
      }
      .toast.err {
        background: var(--red);
      }

      .view {
        display: none;
      }
      .view.on {
        display: block;
      }

      /* HOTSPOT BANNER*/
      .banner {
        display: flex;
        gap: 14px;
        background: var(--amber-soft);
        border-radius: 20px;
        padding: 16px 18px;
        margin-bottom: 20px;
      }
      .banner.info {
        background: var(--soft);
      }
      .banner b {
        display: block;
        font-size: 15.5px;
        font-weight: 700;
        letter-spacing: -0.015em;
        color: var(--ink);
      }
      .banner.info b {
        color: var(--ink);
      }
      .banner p {
        font-size: 14px;
        line-height: 1.5;
        color: var(--ink);
        margin-top: 5px;
        opacity: 0.82;
      }
      .banner code {
        font: inherit;
        font-weight: 600;
        background: transparent;
        padding: 0;
      }

      /* SETTINGS */
      .panel {
        background: var(--card);
        border: 1px solid var(--line);
        border-radius: 22px;
        padding: 16px;
      }
      .panel + .panel {
        margin-top: 12px;
      }
      #cfgfx > .panel,
      #wififx > .panel {
        margin-top: 12px;
      }
      .phead {
        display: flex;
        align-items: flex-start;
        gap: 12px;
      }
      .pico {
        flex: none;
        color: var(--dim);
        padding-top: 1px;
      }
      .pico svg {
        width: 21px;
        height: 21px;
      }
      .pico.red {
        color: var(--red);
      }
      .phead > div {
        flex: 1;
        min-width: 0;
      }
      .phead h4 {
        font-size: 17px;
        font-weight: 700;
        letter-spacing: -0.02em;
      }
      .phead h4 + p {
        font-size: 13.5px;
        color: var(--dim);
        line-height: 1.45;
        margin-top: 3px;
      }

      .field {
        margin: 16px auto 0;
        width: 100%;
        max-width: 420px;
      }
      .field label {
        display: block;
        font-size: 13px;
        font-weight: 600;
        color: var(--dim);
        margin-bottom: 6px;
      }
      .field input {
        width: 100%;
        background: var(--soft);
        border: 1.5px solid transparent;
        border-radius: 13px;
        padding: 12px 14px;
        font: inherit;
        font-size: 16px;
        color: var(--ink);
      }
      .field input:focus {
        border-color: var(--dim);
        outline: none;
        background: var(--card);
      }
      .field small {
        display: block;
        font-size: 12.5px;
        color: var(--dim);
        margin-top: 5px;
        line-height: 1.4;
      }

      .btn {
        display: block;
        min-width: 170px;
        background: var(--btn);
        color: var(--btn-ink);
        border-radius: 13px;
        padding: 12px 22px;
        font-size: 15.5px;
        font-weight: 600;
        margin: 16px auto 0;
        transition: opacity 0.15s;
      }

      .gcard .btn {
        width: 100%;
      }
      .btn:active {
        opacity: 0.8;
      }
      .btn:disabled {
        opacity: 0.45;
        cursor: default;
      }
      .btn.ghost {
        background: var(--soft);
        color: var(--ink);
      }
      .btn.warn {
        background: var(--red-soft);
        color: var(--red);
      }
      .btn.hold3 {
        position: relative;
        overflow: hidden;
      }
      .btn.hold3 .holdbar {
        position: absolute;
        left: 0;
        bottom: 0;
        height: 3px;
        width: 100%;
        background: transparent;
      }
      .btn.hold3 .holdbar i {
        display: block;
        height: 100%;
        width: 0;
        background: var(--red);
      }

      .link,
      .rescan {
        display: flex;
        align-items: center;
        gap: 7px;
        flex: none;
        align-self: center;
        margin: 0;
        padding: 5px 2px;
        white-space: nowrap;
        font-size: 15.5px;
        font-weight: 600;
        color: var(--ink);
      }
      .link.warn {
        color: var(--red);
      }

      .state {
        display: flex;
        align-items: flex-start;
        gap: 9px;
        margin: 14px auto 0;
        width: 100%;
        max-width: 420px;
        font-size: 13.5px;
        font-weight: 600;
        line-height: 1.45;
        color: var(--dim);
      }
      .state::before {
        content: "";
        flex: none;
        width: 8px;
        height: 8px;
        border-radius: 50%;
        margin-top: 5px;
        background: currentColor;
      }
      .state.ok {
        color: var(--green-ink);
      }
      .state.bad {
        color: var(--red);
      }

      /* NETWORK LIST */
      .nets {
        display: flex;
        flex-direction: column;
        gap: 8px;
        margin-top: 15px;
      }
      .net {
        display: flex;
        align-items: center;
        gap: 13px;
        width: 100%;
        text-align: left;
        background: var(--soft);
        border-radius: 15px;
        padding: 13px 15px;
        transition: background 0.15s;
      }
      .net:active {
        background: var(--track);
      }
      .net.sel {
        background: var(--sel);
        color: var(--sel-ink);
      }
      .net b {
        font-size: 15.5px;
        font-weight: 600;
        overflow: hidden;
        text-overflow: ellipsis;
        white-space: nowrap;
      }
      .net .lock {
        color: var(--dim);
        flex: none;
      }
      .net .lock svg {
        width: 12px;
        height: 12px;
      }
      .net.sel .lock {
        color: var(--sel-ink);
      }

      .join {
        background: var(--soft);
        border-radius: 15px;
        padding: 2px 15px 15px;
        margin: -4px 0 4px;
      }
      .join .field {
        margin-top: 12px;
      }
      .join .field input {
        background: var(--card);
      }
      .join .btn {
        margin-top: 12px;
      }

      .sig {
        display: flex;
        align-items: flex-end;
        gap: 2.5px;
        margin-left: auto;
        flex: none;
        height: 16px;
      }
      .phead .sig {
        margin-left: 0;
        align-self: center;
      }
      .sig i {
        width: 3.5px;
        border-radius: 2px;
        background: var(--track);
        display: block;
      }
      .sig i:nth-child(1) {
        height: 5px;
      }
      .sig i:nth-child(2) {
        height: 8.5px;
      }
      .sig i:nth-child(3) {
        height: 12px;
      }
      .sig i:nth-child(4) {
        height: 16px;
      }
      .sig i.f {
        background: var(--green);
      }
      .net.sel .sig i {
        background: var(--track);
      }
      .net.sel .sig i.f {
        background: var(--green);
      }

      .note {
        font-size: 13.5px;
        color: var(--dim);
        margin: 12px auto 0;
        max-width: 420px;
        line-height: 1.5;
      }
      .note.warn {
        color: var(--red);
      }
      .note:empty {
        margin-top: 0;
      }

      .pfx {
        display: flex;
        align-items: center;
        width: 100%;
        background: var(--soft);
        border: 1.5px solid transparent;
        border-radius: 13px;
        padding-left: 14px;
      }
      .pfx:focus-within {
        border-color: var(--dim);
        background: var(--card);
      }
      .pfx > span {
        flex: none;
        font-size: 16px;
        font-weight: 600;
        color: var(--dim);
      }
      .pfx input {
        background: none;
        border: 0;
        padding-left: 2px;
      }
      .pfx input:focus {
        background: none;
        border: 0;
      }

      /* password show icon */
      .pw {
        position: relative;
      }
      .pw input {
        padding-right: 50px;
      }
      .pw .eye {
        position: absolute;
        right: 5px;
        top: 31px;
        width: 38px;
        height: 38px;
        border-radius: 11px;
        display: grid;
        place-items: center;
        color: var(--dim);
      }
      .pw .eye:hover {
        background: var(--track);
      }
      .pw .eye svg {
        width: 19px;
        height: 19px;
      }
      .pw .eye.on {
        color: var(--ink);
      }

      .rescan svg {
        width: 17px;
        height: 17px;
      }
      .rescan:disabled {
        color: var(--dim);
        cursor: default;
      }
      .rescan.busy svg {
        animation: spin 1s linear infinite;
      }

      @media (prefers-reduced-motion: reduce) {
        * {
          animation: none !important;
          transition: none !important;
        }
      }
    </style>
  </head>
  <body>
    <!-- ICONS -->
    <svg width="0" height="0" style="position: absolute" aria-hidden="true">
      <defs>
        <symbol id="i-desktop" viewBox="0 0 96 96">
          <path
            d="M90,0H6A5.9966,5.9966,0,0,0,0,6V66a5.9966,5.9966,0,0,0,6,6H42V84H30a6,6,0,0,0,0,12H66a6,6,0,0,0,0-12H54V72H90a5.9966,5.9966,0,0,0,6-6V6A5.9966,5.9966,0,0,0,90,0ZM84,60H12V12H84Z"
          />
        </symbol>
        <symbol id="i-reload" viewBox="0 0 96 96">
          <path
            d="M94.2422,37.7578a5.9979,5.9979,0,0,0-8.4844,0l-2.61,2.61A36.0347,36.0347,0,0,0,48,12a35.55,35.55,0,0,0-21.6211,7.3594,5.9977,5.9977,0,0,0,7.2422,9.5625A23.6677,23.6677,0,0,1,48,24,23.957,23.957,0,0,1,70.6729,40.4766l-3.97-3.1641a5.9956,5.9956,0,1,0-7.4765,9.375l15.0351,12a5.99,5.99,0,0,0,7.98-.4453l12-12A5.9979,5.9979,0,0,0,94.2422,37.7578Z"
          />
          <path
            d="M62.3789,67.0781A23.6675,23.6675,0,0,1,48,72,23.957,23.957,0,0,1,25.3271,55.5234l3.97,3.1641a5.9956,5.9956,0,1,0,7.4765-9.375l-15.0351-12a6.0071,6.0071,0,0,0-7.98.4453l-12,12a5.9994,5.9994,0,0,0,8.4844,8.4844l2.61-2.61A36.0347,36.0347,0,0,0,48,84a35.55,35.55,0,0,0,21.6211-7.3594,5.9977,5.9977,0,1,0-7.2422-9.5625Z"
          />
        </symbol>
        <symbol id="i-wifi" viewBox="0 0 96 96">
          <path d="M48,60A12,12,0,1,0,60,72,12.0081,12.0081,0,0,0,48,60Z" />
          <path
            d="M22.6055,46.6289A5.9994,5.9994,0,1,0,31.1133,55.09a24.2258,24.2258,0,0,1,33.7734,0,5.9512,5.9512,0,0,0,4.2539,1.77,6,6,0,0,0,4.2539-10.23C59.7773,32.918,36.2227,32.918,22.6055,46.6289Z"
          />
          <path
            d="M90.27,29.7773a59.1412,59.1412,0,0,0-84.539,0,5.9994,5.9994,0,1,0,8.5312,8.4375c18.1172-18.3281,49.3594-18.3281,67.4766,0A5.9994,5.9994,0,1,0,90.27,29.7773Z"
          />
        </symbol>
        <symbol id="i-server" viewBox="0 0 96 96">
          <path
            d="M0,23.8362v48c0,15.7617,24.1465,24,48,24s48-8.2383,48-24v-48C96-7.6873,0-7.6873,0,23.8362Zm84,24c0,4.23-13.67,12-36,12s-36-7.77-36-12V40.0645c9.2527,5.0778,22.5938,7.7717,36,7.7717s26.7473-2.6939,36-7.7717Zm-36-36c22.33,0,36,7.7695,36,12s-13.67,12-36,12-36-7.7695-36-12S25.67,11.8362,48,11.8362Zm0,72c-22.33,0-36-7.77-36-12V64.0645c9.2527,5.0778,22.5938,7.7717,36,7.7717s26.7473-2.6939,36-7.7717v7.7717C84,76.0667,70.33,83.8362,48,83.8362Z"
          />
        </symbol>
        <symbol id="i-globe" viewBox="0 0 96 96">
          <path
            d="M48,0A48,48,0,1,0,96,48,48.0474,48.0474,0,0,0,48,0Zm.1384,83.9861c-.02,0-.0395.0058-.06.0058-1.6055-.5192-4.8238-5.6689-6.9676-15.238,2.2969.13,4.6.2461,6.8892.2461,2.2917,0,4.5989-.1157,6.8994-.2461C52.7761,78.3091,49.6135,83.4543,48.1384,83.9861ZM48,57c-3.0923,0-5.9539-.1333-8.6543-.3442C39.134,53.9553,39,51.0923,39,48s.1333-5.9539.3442-8.6543C42.0447,39.134,44.9077,39,48,39s5.9539.1333,8.6543.3442C56.866,42.0447,57,44.9077,57,48s-.1333,5.9539-.3442,8.6543C53.9553,56.866,51.0923,57,48,57ZM12.0139,48.1384c0-.02-.0058-.0395-.0058-.06.5192-1.6055,5.6689-4.8238,15.238-6.9676C27.1165,43.4077,27,45.7112,27,48c0,2.2917.1157,4.5989.2461,6.8994C17.6909,52.7761,12.5457,49.6135,12.0139,48.1384ZM47.8616,12.0139c.02,0,.0395-.0058.06-.0058,1.6055.5192,4.8238,5.6689,6.9676,15.238C52.5923,27.1165,50.2888,27,48,27c-2.2917,0-4.5989.1157-6.8994.2461C43.2239,17.6909,46.3865,12.5457,47.8616,12.0139ZM68.7539,41.1006c9.5552,2.1233,14.7,5.2859,15.2322,6.761,0,.02.0058.0395.0058.06-.5192,1.6055-5.6689,4.8238-15.238,6.9676.13-2.2969.2461-4.6.2461-6.8892C69,45.7083,68.8843,43.4011,68.7539,41.1006Zm11.2669-9.2095a74.6936,74.6936,0,0,0-12.6819-3.23,74.6936,74.6936,0,0,0-3.23-12.6819A36.0229,36.0229,0,0,1,80.0208,31.8911ZM31.89,15.9792a74.685,74.685,0,0,0-3.2293,12.6819A74.6729,74.6729,0,0,0,15.98,31.89,36.0205,36.0205,0,0,1,31.89,15.9792ZM15.98,64.11a74.6559,74.6559,0,0,0,12.6811,3.2293A74.6559,74.6559,0,0,0,31.89,80.02,36.0152,36.0152,0,0,1,15.98,64.11ZM64.11,80.02a74.6729,74.6729,0,0,0,3.2293-12.6811A74.685,74.685,0,0,0,80.0208,64.11,36.0205,36.0205,0,0,1,64.11,80.02Z"
          />
        </symbol>
        <symbol id="i-clock" viewBox="0 0 96 96">
          <path
            d="M48,0A48,48,0,1,0,96,48,48.055,48.055,0,0,0,48,0Zm0,84A36,36,0,1,1,84,48,36.0393,36.0393,0,0,1,48,84Z"
          />
          <path
            d="M66,42H54V30a6,6,0,0,0-12,0V48a5.9966,5.9966,0,0,0,6,6H66a6,6,0,0,0,0-12Z"
          />
        </symbol>
        <symbol id="i-eye" viewBox="0 0 96 96">
          <path
            d="M48,18C22.5,18,3.4,44.2,2.6,45.3a4.5,4.5,0,0,0,0,5.4C3.4,51.8,22.5,78,48,78S92.6,51.8,93.4,50.7a4.5,4.5,0,0,0,0-5.4C92.6,44.2,73.5,18,48,18Zm0,51C29.8,69,14.2,51.7,10.6,48,14.2,44.3,29.8,27,48,27S81.8,44.3,85.4,48C81.8,51.7,66.2,69,48,69Z"
          />
          <path
            d="M48,33A15,15,0,1,0,63,48,15,15,0,0,0,48,33Zm0,21a6,6,0,1,1,6-6A6,6,0,0,1,48,54Z"
          />
        </symbol>
        <symbol id="i-eye-off" viewBox="0 0 96 96">
          <path
            d="M48,18C22.5,18,3.4,44.2,2.6,45.3a4.5,4.5,0,0,0,0,5.4C3.4,51.8,22.5,78,48,78S92.6,51.8,93.4,50.7a4.5,4.5,0,0,0,0-5.4C92.6,44.2,73.5,18,48,18Zm0,51C29.8,69,14.2,51.7,10.6,48,14.2,44.3,29.8,27,48,27S81.8,44.3,85.4,48C81.8,51.7,66.2,69,48,69Z"
          />
          <path
            d="M48,33A15,15,0,1,0,63,48,15,15,0,0,0,48,33Zm0,21a6,6,0,1,1,6-6A6,6,0,0,1,48,54Z"
          />
          <g
            fill="none"
            stroke="currentColor"
            stroke-width="9"
            stroke-linecap="round"
          >
            <path d="M17,79,79,17" />
          </g>
        </symbol>
        <symbol id="i-lock" viewBox="0 0 96 96">
          <path
            d="M72,36H70.5V25.5a22.5,22.5,0,0,0-45,0V36H24a9.0106,9.0106,0,0,0-9,9V87a9.0106,9.0106,0,0,0,9,9H72a9.0106,9.0106,0,0,0,9-9V45A9.0106,9.0106,0,0,0,72,36ZM37.5,25.5a10.5,10.5,0,0,1,21,0V36h-21ZM69,84H27V48H69Z"
          />
        </symbol>
        <symbol id="i-gear" viewBox="0 0 96 96">
          <path
            d="M48,17A31,31,0,1,0,79,48,31,31,0,0,0,48,17ZM48,67A19,19,0,1,1,67,48,19,19,0,0,1,48,67Z"
          />
          <rect x="42" y="2" width="12" height="17" rx="5" />
          <rect
            x="42"
            y="2"
            width="12"
            height="17"
            rx="5"
            transform="rotate(45 48 48)"
          />
          <rect
            x="42"
            y="2"
            width="12"
            height="17"
            rx="5"
            transform="rotate(90 48 48)"
          />
          <rect
            x="42"
            y="2"
            width="12"
            height="17"
            rx="5"
            transform="rotate(135 48 48)"
          />
          <rect
            x="42"
            y="2"
            width="12"
            height="17"
            rx="5"
            transform="rotate(180 48 48)"
          />
          <rect
            x="42"
            y="2"
            width="12"
            height="17"
            rx="5"
            transform="rotate(225 48 48)"
          />
          <rect
            x="42"
            y="2"
            width="12"
            height="17"
            rx="5"
            transform="rotate(270 48 48)"
          />
          <rect
            x="42"
            y="2"
            width="12"
            height="17"
            rx="5"
            transform="rotate(315 48 48)"
          />
        </symbol>
        <symbol id="i-power" viewBox="0 0 96 96">
          <g
            fill="none"
            stroke="currentColor"
            stroke-width="12"
            stroke-linecap="round"
          >
            <path d="M48,12V44" />
            <path d="M27.4,29.5A32,32,0,1,0,68.6,29.5" />
          </g>
        </symbol>
        <symbol id="i-alert" viewBox="0 0 96 96">
          <g
            fill="none"
            stroke="currentColor"
            stroke-width="12"
            stroke-linecap="round"
            stroke-linejoin="round"
          >
            <path d="M48,16,88,82H8Z" />
            <path d="M48,42V58" />
          </g>
          <circle cx="48" cy="70" r="5.5" />
        </symbol>
        <symbol id="i-chev" viewBox="0 0 96 96">
          <g
            fill="none"
            stroke="currentColor"
            stroke-width="12"
            stroke-linecap="round"
            stroke-linejoin="round"
          >
            <path d="M37,20,65,48,37,76" />
          </g>
        </symbol>
        <symbol id="i-logout" viewBox="0 0 96 96">
          <g
            fill="none"
            stroke="currentColor"
            stroke-width="12"
            stroke-linecap="round"
            stroke-linejoin="round"
          >
            <path d="M56,18H74a6,6,0,0,1,6,6V72a6,6,0,0,1-6,6H56" />
            <path d="M16,48H58" />
            <path d="M42,32,58,48,42,64" />
          </g>
        </symbol>
      </defs>
    </svg>

    <div class="boot" id="boot"><div class="spin"></div></div>

    <!-- LOGIN -->
    <div class="gate" id="gate" hidden>
      <div class="gcard">
        <div class="lang">
          <button data-lang="tr">TR</button><button data-lang="en">EN</button>
        </div>
        <div class="ghead">
          <h1>UYANIK</h1>
          <p data-t="gate.sub"></p>
        </div>
        <div class="field pw">
          <label for="g-pw" data-t="gate.pw"></label>
          <input
            id="g-pw"
            type="password"
            autocomplete="current-password"
            data-t-ph="gate.ph"
          />
          <button type="button" class="eye" data-eye="g-pw">
            <svg><use href="#i-eye" /></svg>
          </button>
        </div>
        <label class="chk"
          ><input type="checkbox" id="g-rem" checked /><span
            data-t="gate.remember"
          ></span
        ></label>
        <button class="btn" id="g-go" data-t="gate.go"></button>
        <p class="note" id="g-msg"></p>
        <p class="ghint" data-t="gate.hint"></p>
      </div>
    </div>

    <!-- PANEL -->
    <div class="shell" id="shell" hidden>
      <nav class="rail" id="rail">
        <div class="brand"><b>UYANIK</b></div>
        <button data-view="v-dev" class="on">
          <svg><use href="#i-desktop" /></svg><span data-t="nav.dev"></span>
        </button>
        <button data-view="v-wifi">
          <svg><use href="#i-wifi" /></svg><span data-t="nav.wifi"></span>
        </button>
        <button data-view="v-cfg">
          <svg><use href="#i-gear" /></svg><span data-t="nav.cfg"></span>
        </button>
      </nav>

      <div class="main">
        <div class="app">
          <div class="mbrand"><b>UYANIK</b></div>

          <header class="top">
            <div><h1 id="vtitle"></h1></div>
            <div class="tools">
              <div class="live" id="live">
                <i></i><span id="livetxt"></span>
              </div>
              <button class="iconbtn" id="logout" data-t-aria="cfg.logout">
                <svg><use href="#i-logout" /></svg>
              </button>
              <div class="lang">
                <button data-lang="tr">TR</button
                ><button data-lang="en">EN</button>
              </div>
            </div>
          </header>

          <div class="banner" id="hs" hidden>
            <div>
              <b id="hs-t"></b>
              <p id="hs-p"></p>
            </div>
          </div>

          <!-- DEVICE -->
          <div class="view on" id="v-dev">
            <div class="cols">
              <div>
                <section class="device" id="device">
                  <div class="drow">
                    <span class="dico"
                      ><svg><use href="#i-desktop" /></svg
                    ></span>
                    <button
                      class="sw"
                      id="sw"
                      role="switch"
                      aria-checked="false"
                    ></button>
                  </div>
                  <h2 id="pcname"></h2>
                  <p id="sub"></p>
                </section>

                <h3 data-t="dev.cmds"></h3>
                <div class="cmds">
                  <button class="cmd" data-cmd="reset" data-ms="300">
                    <span class="cico"
                      ><svg><use href="#i-reload" /></svg
                    ></span>
                    <span class="ctxt"
                      ><b data-t="cmd.reset"></b></span>
                    <span class="cgo"
                      ><svg><use href="#i-chev" /></svg
                    ></span>
                  </button>
                  <button
                    class="cmd hot"
                    data-cmd="power_long"
                    data-ms="6000"
                    data-hold="1"
                  >
                    <span class="fill" id="holdbar"></span>
                    <span class="cico"
                      ><svg><use href="#i-power" /></svg
                    ></span>
                    <span class="ctxt"
                      ><b data-t="cmd.force"></b
                      ><i id="holdlbl" data-t="cmd.forceIdle"></i
                    ></span>
                    <span class="cgo"
                      ><svg><use href="#i-chev" /></svg
                    ></span>
                  </button>
                </div>
              </div>

              <div>
                <h3 data-t="dev.conn"></h3>
                <div class="stats">
                  <div class="stat">
                    <svg><use href="#i-wifi" /></svg>
                    <div>
                      <span data-t="conn.rssi"></span><b id="rssi">-</b>
                    </div>
                  </div>
                  <div class="stat">
                    <svg><use href="#i-server" /></svg>
                    <div><span data-t="conn.lip"></span><b id="lip">-</b></div>
                  </div>
                  <div class="stat">
                    <svg><use href="#i-globe" /></svg>
                    <div><span data-t="conn.wip"></span><b id="wip">-</b></div>
                  </div>
                  <div class="stat">
                    <svg><use href="#i-clock" /></svg>
                    <div><span data-t="conn.up"></span><b id="up">-</b></div>
                  </div>
                </div>
              </div>
            </div>
          </div>

          <!-- WIFI -->
          <div class="view" id="v-wifi">
            <section class="panel">
              <div class="phead">
                <span class="pico"
                  ><svg><use href="#i-wifi" /></svg
                ></span>
                <div>
                  <h4 id="w-cur">-</h4>
                  <p id="w-cur2"></p>
                </div>
                <span class="sig" id="w-sig" hidden
                  ><i></i><i></i><i></i><i></i
                ></span>
                <button
                  class="link warn"
                  id="w-forget"
                  data-t="wifi.forget"
                  hidden
                ></button>
              </div>
            </section>

            <section class="panel">
              <div class="phead">
                <span class="pico"
                  ><svg><use href="#i-globe" /></svg
                ></span>
                <div>
                  <h4 data-t="wifi.listT"></h4>
                  <p data-t="wifi.listSub"></p>
                </div>
                <button class="rescan" id="w-scan">
                  <svg><use href="#i-reload" /></svg><span></span>
                </button>
              </div>

              <div class="nets" id="w-list"></div>

              <div id="w-formhome">
                <div class="join" id="w-form" hidden>
                  <div class="field pw">
                    <label for="w-pw" data-t="wifi.pw"></label>
                    <input
                      type="password"
                      id="w-pw"
                      autocomplete="off"
                      data-t-ph="wifi.pwPh"
                    />
                    <button type="button" class="eye" data-eye="w-pw">
                      <svg><use href="#i-eye" /></svg>
                    </button>
                  </div>
                  <button class="btn" id="w-go" data-t="wifi.connect"></button>
                </div>
              </div>

              <p class="note" id="w-msg"></p>
            </section>

            <div id="wififx"></div>
          </div>

          <!-- SETTINGS -->
          <div class="view" id="v-cfg">
            <section class="panel">
              <div class="phead">
                <span class="pico"
                  ><svg><use href="#i-desktop" /></svg
                ></span>
                <div><h4 data-t="cfg.devT"></h4></div>
              </div>
              <div class="field">
                <label for="c-pcname" data-t="cfg.pcName"></label>
                <input id="c-pcname" autocomplete="off" data-t-ph="dev.pc" />
                <small data-t="cfg.pcNameHint"></small>
              </div>
              <div class="field">
                <label for="c-name" data-t="cfg.name"></label>
                <input id="c-name" autocomplete="off" placeholder="uyanik" />
                <small data-t="cfg.nameHint"></small>
              </div>
              <button class="btn" data-save data-t="cfg.save"></button>
            </section>

            <section class="panel">
              <div class="phead">
                <span class="pico"
                  ><svg><use href="#i-wifi" /></svg
                ></span>
                <div>
                  <h4 data-t="cfg.apT"></h4>
                  <p data-t="cfg.apSub"></p>
                </div>
              </div>
              <div class="field">
                <label for="c-apsfx" data-t="cfg.apName"></label>
                <div class="pfx">
                  <span>UYANIK-</span><input id="c-apsfx" autocomplete="off" />
                </div>
                <small data-t="cfg.apHint"></small>
              </div>
              <div class="field pw">
                <label for="c-appw" data-t="cfg.apPw"></label>
                <input
                  id="c-appw"
                  type="password"
                  autocomplete="new-password"
                  data-t-ph="cfg.apPwPh"
                />
                <button type="button" class="eye" data-eye="c-appw">
                  <svg><use href="#i-eye" /></svg>
                </button>
                <small data-t="cfg.apPwHint"></small>
              </div>
              <button class="btn" data-save data-t="cfg.save"></button>
            </section>

            <section class="panel">
              <div class="phead">
                <span class="pico"
                  ><svg><use href="#i-server" /></svg
                ></span>
                <div>
                  <h4 data-t="cfg.fbT"></h4>
                  <p data-t="cfg.fbSub"></p>
                </div>
              </div>

              <div class="state" id="c-state"><span></span></div>

              <div class="field">
                <label for="c-key" data-t="cfg.key"></label>
                <input id="c-key" autocomplete="off" />
              </div>
              <div class="field">
                <label for="c-url" data-t="cfg.url"></label>
                <input
                  id="c-url"
                  autocomplete="off"
                  placeholder="https://…firebasedatabase.app"
                />
              </div>
              <div class="field">
                <label for="c-mail" data-t="cfg.mail"></label>
                <input id="c-mail" type="email" autocomplete="off" />
              </div>
              <div class="field pw">
                <label for="c-pw" data-t="cfg.pw"></label>
                <input
                  id="c-pw"
                  type="password"
                  autocomplete="new-password"
                  data-t-ph="cfg.pwPh"
                />
                <button type="button" class="eye" data-eye="c-pw">
                  <svg><use href="#i-eye" /></svg>
                </button>
                <small data-t="cfg.pwHint"></small>
              </div>
              <p class="note" id="c-err"></p>
              <button class="btn" data-save data-t="cfg.save"></button>
            </section>

            <section class="panel">
              <div class="phead">
                <span class="pico"
                  ><svg><use href="#i-lock" /></svg
                ></span>
                <div>
                  <h4 data-t="cfg.ppwT"></h4>
                  <p data-t="cfg.ppwSub"></p>
                </div>
              </div>
              <div class="field pw">
                <label for="p-cur" data-t="cfg.ppwCur"></label>
                <input
                  id="p-cur"
                  type="password"
                  autocomplete="current-password"
                />
                <button type="button" class="eye" data-eye="p-cur">
                  <svg><use href="#i-eye" /></svg>
                </button>
              </div>
              <div class="field pw">
                <label for="p-new" data-t="cfg.ppwNew"></label>
                <input id="p-new" type="password" autocomplete="new-password" />
                <button type="button" class="eye" data-eye="p-new">
                  <svg><use href="#i-eye" /></svg>
                </button>
                <small data-t="cfg.ppwHint"></small>
              </div>
              <p class="note" id="p-msg"></p>
              <button class="btn" id="p-go" data-t="cfg.ppwBtn"></button>
            </section>

            <section class="panel">
              <div class="phead">
                <span class="pico"
                  ><svg><use href="#i-reload" /></svg
                ></span>
                <div>
                  <h4 data-t="cfg.rsT"></h4>
                  <p data-t="cfg.rsSub"></p>
                </div>
              </div>
              <button class="btn ghost" id="rs" data-t="cfg.rsBtn"></button>
            </section>

            <!-- FACTORY RESET -->
            <div id="cfgfx">
              <section class="panel" id="p-fx">
                <div class="phead">
                  <span class="pico red"
                    ><svg><use href="#i-alert" /></svg
                  ></span>
                  <div>
                    <h4 data-t="cfg.fxT"></h4>
                    <p data-t="cfg.fxSub"></p>
                  </div>
                </div>
                <button class="btn warn hold3" id="fx">
                  <span data-t="cfg.fxBtn"></span>
                  <div class="holdbar"><i id="fxbar"></i></div>
                </button>
              </section>
            </div>
          </div>
        </div>
      </div>
    </div>

    <div class="toast" id="toast"><span id="tmsg"></span></div>

    <script>
      var $ = function (s) {
        return document.querySelector(s);
      };

      function esc(t) {
        return String(t == null ? "" : t)
          .replace(/&/g, "&amp;")
          .replace(/</g, "&lt;")
          .replace(/>/g, "&gt;");
      }

      function lsGet(k) {
        try {
          return localStorage.getItem(k) || "";
        } catch (e) {
          return "";
        }
      }
      function lsSet(k, v) {
        try {
          localStorage.setItem(k, v);
        } catch (e) {}
      }
      function lsDel(k) {
        try {
          localStorage.removeItem(k);
        } catch (e) {}
      }

      /* LANGUAGES */
      var LANG = {
        tr: {
          "nav.dev": "Cihaz",
          "nav.wifi": "Wi-Fi",
          "nav.cfg": "Ayarlar",

          "live.online": "Çevrimiçi",
          "live.local": "Lokal",
          "live.hotspot": "Hotspot",
          "live.offline": "Çevrimdışı",

          "dev.pc": "Bilgisayar",
          "dev.on": "Açık",
          "dev.off": "Kapalı",
          "dev.unreachable": "Cihaza ulaşılamıyor",
          "dev.swAria": "Bilgisayarı aç veya kapat",
          "dev.pressing": "güç düğmesine basılıyor…",
          "dev.resetting": "yeniden başlatılıyor…",
          "dev.holding": "güç düğmesi basılı tutuluyor…",
          "dev.cmds": "Komutlar",
          "dev.conn": "Bağlantı",

          "cmd.reset": "Yeniden Başlat",
          "cmd.resetSub": "",
          "cmd.force": "Zorla Kapat",
          "cmd.forceIdle": "Basılı tut",
          "cmd.holding": "{0} saniye bırakma",
          "cmd.released": "Bırakıldı, komut gönderilmedi",
          "cmd.ok": "Komut uygulandı",
          "cmd.busy": "Önceki komut sürüyor",
          "cmd.fail": "Komut gönderilemedi",

          "conn.rssi": "Sinyal",
          "conn.lip": "Lokal IP",
          "conn.wip": "IP",
          "conn.up": "Çalışma Süresi",
          "conn.uptime": "{0} sa {1} dk",

          "wifi.noneT": "Ağ seçilmedi",
          "wifi.none": "Kayıtlı ağ yok",
          "wifi.connOn": "Bağlı",
          "wifi.portalCur": "Kurulum ağı yayında. Aşağıdan ev ağını seç.",
          "wifi.onlyCurrent": "Menzilde başka ağ yok.",
          "wifi.forget": "Ağı unut",
          "wifi.forgetAsk":
            "Kayıtlı Wi-Fi ağı silinecek ve cihaz kendi ağını açacak. Devam?",
          "wifi.listT": "Kullanılabilir ağlar",
          "wifi.listSub": "Bir ağa dokun, parolasını yaz, bağlan.",
          "wifi.scan": "Tara",
          "wifi.scanning": "Taranıyor",
          "wifi.empty": "Ağ bulunamadı.",
          "wifi.pw": "Parola",
          "wifi.pwPh": "ağ parolası",
          "wifi.connect": "Bağlan",
          "wifi.trying": "Bağlanılıyor…",
          "wifi.tryingN": "Bağlanılıyor… ({0} sn)",
          "wifi.sendFail": "İstek gönderilemedi.",
          "wifi.moved": "Cihaz yeni ağa geçmiş olabilir, panele o ağdan bağlan.",
          "res.ok": "Bağlandı: {0}",
          "res.sifre": "Parola yanlış. Tekrar dene.",
          "res.bulunamadi": "Ağ bulunamadı. Menzilde mi, adı doğru mu?",
          "res.zamanasimi": "Ağ yanıt vermedi. Sinyal zayıf olabilir.",
          "res.adbos": "Önce bir ağ seç.",
          "res.aduzun": "Ağ adı 32 karakteri aşıyor.",
          "res.sifreuzun": "Parola 64 karakteri aşıyor.",
          "res.other": "Bağlanılamadı.",
          "res.portal": "Cihaz hotspot modunda kaldı.",

          "cfg.devT": "Cihaz",
          "cfg.pcName": "Bilgisayar adı",
          "cfg.pcNameHint":
            'Cihaz sekmesinde bu ad görünür. Boş bırakırsan "Bilgisayar" yazar.',
          "cfg.name": "Cihaz adı",
          "cfg.nameHint":
            "Veritabanındaki kök düğümün ve uyanik.local adresinin adı, komutlar buradan geçiyor. Değiştirmen gerekmiyor. Yalnızca harf, rakam, _ ve -.",
          "cfg.apT": "Hotspot",
          "cfg.apSub":
            "Cihaz bir ağa bağlanamadığında kendi kurulum ağını yayınlar. Bu ağın adı. Parolası panel parolasıyla aynıdır.",
          "cfg.apName": "Ağ adı",
          "cfg.apHint": "Harf, rakam, _ ve -. En fazla 16 karakter.",
          "cfg.apPw": "Ağ parolası",
          "cfg.apPwPh": "değiştirmemek için boş bırak",
          "cfg.apPwHint":
            "8-32 karakter. Bu ağa bağlanırken sorulur. Hiç vermezsen panel parolasıyla aynıdır.",
          "cfg.fbT": "Uzaktan erişim",
          "cfg.fbSub":
            "Firebase üzerinden başka ağlardan kontrol. Boş bırakırsan yalnızca yerel panel çalışır.",
          "cfg.key": "Firebase Web API Key",
          "cfg.url": "Realtime Database adresi",
          "cfg.mail": "Cihaz e-postası",
          "cfg.pw": "Cihaz parolası",
          "cfg.pwPh": "değiştirmemek için boş bırak",
          "cfg.pwHint": "Kayıtlı parola burada gösterilmez.",
          "cfg.save": "Kaydet",
          "cfg.saved": "Ayarlar kaydedildi",
          "cfg.saveFail": "Kaydedilemedi",
          "cfg.readFail": "Ayarlar okunamadı",
          "cfg.remoteOk": "Bağlı, komutlar her yerden çalışıyor.",
          "cfg.remoteOff": "Bağlı değil.",
          "cfg.remoteWait": "Ayarlar bekleniyor.",
          "cfg.remoteClock": "Saat ayarlanıyor, NTP bekleniyor.",
          "cfg.logout": "Çıkış yap",
          "cfg.logoutAsk":
            "Çıkış yapılacak. Açık olan tüm paneller de düşer ve kayıtlı parola silinir; geri girmek için panel parolası gerekecek. Devam?",
          "cfg.ppwT": "Panel parolası",
          "cfg.ppwSub":
            "Hem panele girişte hem hotspot ağına bağlanırken kullanılır, ikisi aynı parola. Unutursan fabrika sıfırlama ilk açılıştaki parolayı geri getirir.",
          "cfg.ppwCur": "Mevcut parola",
          "cfg.ppwNew": "Yeni parola",
          "cfg.ppwHint": "En az 8, en fazla 32 karakter.",
          "cfg.ppwBtn": "Parolayı değiştir",
          "cfg.ppwOk":
            "Parola değiştirildi. Diğer cihazlarda yeniden giriş gerekir.",
          "cfg.rsT": "Yeniden başlat",
          "cfg.rsSub":
            "Cihazı kapatıp açar. Hiçbir ayar silinmez, bilgisayara dokunulmaz.",
          "cfg.rsBtn": "Cihazı yeniden başlat",
          "cfg.rsAsk":
            "UYANIK yeniden başlatılacak. Panel yaklaşık 10 saniye kapalı kalır. Devam?",
          "cfg.fxT": "Fabrika ayarlarına dön",
          "cfg.fxSub":
            "Wi-Fi ve Firebase ayarları silinir, cihaz yeniden başlar. Cihazın ilk açılışında üretilen parolalar değişmez.",
          "cfg.fxBtn": "Basılı tut,  3 saniye",

          "gate.sub": "Panele girmek için parolanı yaz.",
          "gate.pw": "Parola",
          "gate.ph": "cihaz parolası",
          "gate.remember": "Beni hatırla",
          "gate.go": "Giriş",
          "gate.trying": "Kontrol ediliyor…",
          "gate.bad": "Parola yanlış.",
          "gate.empty": "Parola boş.",
          "gate.locked": "Çok fazla deneme. {0} saniye bekle.",
          "gate.netfail": "Cihaza ulaşılamadı.",
          "gate.hint":
            "Kurulumda kaydettiğin cihaz parolası. Kullanıcı adı gerekmiyor.",

          "hs.closingT": "Bağlandı, hotspot {0} sn içinde kapanıyor",
          "hs.closingP":
            "Telefonunu <b>{0}</b> ağına al, sonra panele buradan devam et:<br><code>{1}</code> <code>{2}</code>",
          "hs.portalT": "Hotspot modu",
          "hs.portalP":
            "Cihaz bir ağa bağlı değil, kendi ağını yayınlıyor: <code>{0}</code><br>Bağlandıktan sonra bu panel kapanır ve komutlar yeniden çalışır.",
          "hs.closedT": "Hotspot kapandı",
          "hs.closedP":
            "Telefonun hâlâ cihazın ağında olabilir. <b>{0}</b> ağına geç ve panele buradan devam et:<br><code>{1}</code> <code>{2}</code>",
          "hs.rsT": "Cihaz yeniden başlıyor",
          "hs.rsP":
            "Panel birkaç saniye içinde kendiliğinden geri gelir. Bilgisayara dokunulmadı.",
          "hs.fxT": "Fabrika ayarlarına dönülüyor",
          "hs.fxP":
            "Cihaz yeniden başlıyor ve kendi ağını açacak: <code>{0}</code><br>Parola <b>değişmedi</b> ilk açılıştakiyle aynı.",
          "hs.forgotT": "Ağdan çıkıldı",
          "hs.forgotP":
            "Cihaz artık kendi ağını yayınlıyor: <code>{0}</code><br>Telefonundan o ağa bağlan, panel kendiliğinden açılır. Parola, kurulumda kaydettiğin parola",

          "a.show": "Parolayı göster",
          "a.hide": "Parolayı gizle",
        },
        en: {
          "nav.dev": "Device",
          "nav.wifi": "Wi-Fi",
          "nav.cfg": "Settings",

          "live.online": "Online",
          "live.local": "Local only",
          "live.hotspot": "Hotspot",
          "live.offline": "Offline",

          "dev.pc": "Computer",
          "dev.on": "On",
          "dev.off": "Off",
          "dev.unreachable": "Device unreachable",
          "dev.swAria": "Turn the computer on or off",
          "dev.pressing": "pressing the power button…",
          "dev.resetting": "restarting…",
          "dev.holding": "holding the power button…",
          "dev.cmds": "Commands",
          "dev.conn": "Connection",

          "cmd.reset": "Restart",
          "cmd.resetSub": "",
          "cmd.force": "Force Off",
          "cmd.forceIdle": "Hold",
          "cmd.holding": "keep holding, {0}s",
          "cmd.released": "Released, command not sent",
          "cmd.ok": "Command sent",
          "cmd.busy": "Previous command still running",
          "cmd.fail": "Could not send the command",

          "conn.rssi": "Signal",
          "conn.lip": "LocaL IP",
          "conn.wip": "Public IP",
          "conn.up": "Uptime",
          "conn.uptime": "{0} h {1} m",

          "wifi.noneT": "No network selected",
          "wifi.none": "No saved network",
          "wifi.connOn": "Connected",
          "wifi.portalCur":
            "Setup network is live. Pick your home network below.",
          "wifi.onlyCurrent": "No other networks in range.",
          "wifi.forget": "Forget network",
          "wifi.forgetAsk":
            "The saved Wi-Fi network will be erased and the device will open its own network. Continue?",
          "wifi.listT": "Available networks",
          "wifi.listSub": "Tap a network, type its password, connect.",
          "wifi.scan": "Scan",
          "wifi.scanning": "Scanning",
          "wifi.empty": "No networks found.",
          "wifi.pw": "Password",
          "wifi.pwPh": "network password",
          "wifi.connect": "Connect",
          "wifi.trying": "Connecting…",
          "wifi.tryingN": "Connecting… ({0}s)",
          "wifi.sendFail": "Could not send the request.",
          "wifi.moved":
            "The device may have moved to the new network, open the panel from there.",

          "res.ok": "Connected: {0}",
          "res.sifre": "Wrong password. Try again.",
          "res.bulunamadi": "Network not found. In range? Name correct?",
          "res.zamanasimi": "The network did not respond. Signal may be weak.",
          "res.adbos": "Pick a network first.",
          "res.aduzun": "Network name is longer than 32 characters.",
          "res.sifreuzun": "Password is longer than 64 characters.",
          "res.other": "Could not connect.",
          "res.portal": "The device stayed in hotspot mode.",

          "cfg.devT": "Device",
          "cfg.pcName": "Computer name",
          "cfg.pcNameHint":
            'Shown on the Device tab. Leave empty and it reads "Computer".',
          "cfg.name": "Device name",
          "cfg.nameHint":
            "Name of the root node in the database and of the .local address, commands travel through it. You normally don't need to change it. Letters, digits, _ and - only.",
          "cfg.apT": "Hotspot",
          "cfg.apSub":
            "When the device cannot join a network it broadcasts its own setup network. This is its name. Its password is the panel password.",
          "cfg.apName": "Network name",
          "cfg.apHint": "Letters, digits, _ and -. At most 16 characters.",
          "cfg.apPw": "Network password",
          "cfg.apPwPh": "leave empty to keep the current one",
          "cfg.apPwHint":
            "8-32 characters. Asked when joining this network. If you never set one it matches the panel password.",
          "cfg.fbT": "Remote access",
          "cfg.fbSub":
            "Control from other networks over Firebase. Leave empty to keep the device local-only.",
          "cfg.key": "Firebase Web API key",
          "cfg.url": "Realtime Database URL",
          "cfg.mail": "Device e-mail",
          "cfg.pw": "Device password",
          "cfg.pwPh": "leave empty to keep the current one",
          "cfg.pwHint": "The saved password is never shown here.",
          "cfg.save": "Save",
          "cfg.saved": "Settings saved",
          "cfg.saveFail": "Could not save",
          "cfg.readFail": "Could not read the settings",
          "cfg.remoteOk": "Connected, commands work from anywhere.",
          "cfg.remoteOff": "Not connected.",
          "cfg.remoteWait": "Waiting for settings.",
          "cfg.remoteClock": "Setting the clock, waiting for NTP.",
          "cfg.logout": "Sign out",
          "cfg.logoutAsk":
            "You will be signed out. Every open panel drops too and the saved password is erased; you will need the panel password to get back in. Continue?",
          "cfg.ppwT": "Panel password",
          "cfg.ppwSub":
            "Used both to sign in here and to join the hotspot network, one password for both. If you forget it, a factory reset restores the one from the first boot.",
          "cfg.ppwCur": "Current password",
          "cfg.ppwNew": "New password",
          "cfg.ppwHint": "Between 8 and 32 characters.",
          "cfg.ppwBtn": "Change password",
          "cfg.ppwOk": "Password changed. Other devices must sign in again.",
          "cfg.rsT": "Restart",
          "cfg.rsSub":
            "Power-cycles the device. Nothing is erased and the computer is untouched.",
          "cfg.rsBtn": "Restart device",
          "cfg.rsAsk":
            "UYANIK will restart. The panel stays down for about 10 seconds. Continue?",
          "cfg.fxT": "Factory reset",
          "cfg.fxSub":
            "Wi-Fi and Firebase settings are erased and the device restarts. The passwords generated on the device's first boot do not change.",
          "cfg.fxBtn": "Hold, 3 seconds",

          "gate.sub": "Type your password to open the panel.",
          "gate.pw": "Password",
          "gate.ph": "device password",
          "gate.remember": "Remember me",
          "gate.go": "Sign in",
          "gate.trying": "Checking…",
          "gate.bad": "Wrong password.",
          "gate.empty": "Password is empty.",
          "gate.locked": "Too many attempts. Wait {0} seconds.",
          "gate.netfail": "Could not reach the device.",
          "gate.hint":
            "The device password you saved during setup. No username needed.",

          "hs.closingT": "Connected, hotspot closes in {0}s",
          "hs.closingP":
            "Move your phone to <b>{0}</b>, then continue here:<br><code>{1}</code> <code>{2}</code>",
          "hs.portalT": "Hotspot mode",
          "hs.portalP":
            "The device is not on any network, so it broadcasts its own: <code>{0}</code><br>Once it joins a network this panel closes and commands work again.",
          "hs.closedT": "Hotspot closed",
          "hs.closedP":
            "Your phone may still be on the device network. Switch to <b>{0}</b> and continue here:<br><code>{1}</code> <code>{2}</code>",
          "hs.rsT": "Device is restarting",
          "hs.rsP":
            "The panel comes back on its own in a few seconds. The computer was not touched.",
          "hs.fxT": "Restoring factory settings",
          "hs.fxP":
            "The device now broadcasts its own network: <code>{0}</code><br>Join it from your phone and the panel opens by itself.",
          "hs.forgotT": "Left the network",
          "hs.forgotP":
            "The device now broadcasts its own network: <code>{0}</code><br>Join it from your phone and the panel opens by itself.",

          "a.show": "Show password",
          "a.hide": "Hide password",
        },
      };

      /* 
        Firmware hata metinleri Türkçe (settings.cpp, rtdb.cpp).
        Firmware error messages in Turkish (settings.cpp, rtdb.cpp).
      */

      var ERR = {
        "adres bos": "The address is empty",
        "adres cok uzun": "The address is too long",
        "https olmali": "Must start with https",
        "adreste gecersiz karakter": "Invalid character in the address",
        "eksik alan adi": "Incomplete domain name",
        "Firebase adresi degil": "Not a Firebase address",
        "cihaz adi bos": "The device name is empty",
        "cihaz adi cok uzun": "The device name is too long",
        "cihaz adinda sadece harf, rakam, _ ve - olabilir":
          "The device name may contain only letters, digits, _ and -",
        "ayarlar kaydedilemedi": "Could not save the settings",
        "Firebase ayarlari eksik": "Firebase settings are incomplete",
        "kimlik bilgileri eksik": "Credentials are missing",
        "yetki reddedildi": "Access denied",
        "gecersiz yol": "Invalid path",
        "okuma hatasi": "Read error",
        "yazma hatasi": "Write error",
        "cevap cok buyuk": "Response too large",
        "govde okunamadi": "Could not read the response body",
        "zaman damgasi yazilamadi": "Could not write the timestamp",
        "mevcut parola yanlis": "The current password is wrong",
        "parola en az 8 karakter olmali":
          "The password must be at least 8 characters",
        "parola en fazla 32 karakter olabilir":
          "The password can be at most 32 characters",
        "bilgisayar adi cok uzun": "The computer name is too long",
        "bilgisayar adinda gecersiz karakter":
          "Invalid character in the computer name",
        "hotspot adi bos": "The hotspot name is empty",
        "hotspot adi cok uzun": "The hotspot name is too long",
        "hotspot adinda sadece harf, rakam, _ ve - olabilir":
          "The hotspot name may contain only letters, digits, _ and -",
        "hotspot parolasi en az 8 karakter olmali":
          "The hotspot password must be at least 8 characters",
        "hotspot parolasi en fazla 32 karakter olabilir":
          "The hotspot password can be at most 32 characters",
        "hotspot parolasinda gecersiz karakter":
          "Invalid character in the hotspot password",
        "parolada gecersiz karakter": "Invalid character in the password",
      };

      var lang = lsGet("uyanik.lang");
      if (lang !== "tr" && lang !== "en")
        lang =
          (navigator.language || "tr").toLowerCase().indexOf("tr") === 0
            ? "tr"
            : "en";

      function t(k, a) {
        var s = LANG[lang][k];
        if (s == null) s = LANG.tr[k];
        if (s == null) return k;
        if (a)
          for (var i = 0; i < a.length; i++)
            s = s.split("{" + i + "}").join(a[i]);
        return s;
      }
      function terr(s) {
        if (!s) return "";
        return lang === "en" && ERR[s] ? ERR[s] : s;
      }

      function setLang(l) {
        if ((l !== "tr" && l !== "en") || l === lang) return;
        lang = l;
        lsSet("uyanik.lang", l);
        applyLang();
      }

      function applyLang() {
        document.documentElement.lang = lang;

        document.querySelectorAll("[data-t]").forEach(function (el) {
          el.textContent = t(el.dataset.t);
        });
        document.querySelectorAll("[data-t-ph]").forEach(function (el) {
          el.placeholder = t(el.dataset.tPh);
        });
        document.querySelectorAll("[data-t-aria]").forEach(function (el) {
          el.setAttribute("aria-label", t(el.dataset.tAria));
        });
        document.querySelectorAll("[data-lang]").forEach(function (b) {
          b.classList.toggle("on", b.dataset.lang === lang);
        });

        $("#sw").setAttribute("aria-label", t("dev.swAria"));

        document.querySelectorAll("[data-eye]").forEach(function (b) {
          var hid = document.getElementById(b.dataset.eye).type === "password";
          b.setAttribute("aria-label", t(hid ? "a.show" : "a.hide"));
        });

        $("#w-scan").querySelector("span").textContent = t(
          scanBusy ? "wifi.scanning" : "wifi.scan",
        );

        $("#vtitle").textContent = t("nav." + view.slice(2));
        if (lastData) paint(lastData);
        else renderBanner();
        renderWmsg();
        renderGmsg();
        renderCfgErr();
        renderPmsg();
        if (netCache) drawNets(netCache);
      }

      document.querySelectorAll("[data-lang]").forEach(function (b) {
        b.addEventListener("click", function () {
          setLang(b.dataset.lang);
        });
      });

      /* ID */
      var TOKEN = lsGet("uyanik.tok"),
        REMPW = lsGet("uyanik.pw");

      function raw(path, opts) {
        opts = Object.assign({ cache: "no-store" }, opts || {});
        var h = Object.assign({ "X-Uyanik": "1" }, opts.headers || {});
        if (TOKEN) h["X-Uyanik-Token"] = TOKEN;
        opts.headers = h;
        return fetch(path, opts);
      }

      function api(path, opts) {
        return raw(path, opts).then(function (r) {
          if (r.status !== 401) return r;
          return relogin().then(function (ok) {
            if (!ok) {
              showGate();
              return r;
            }
            return raw(path, opts);
          });
        });
      }

      var reloginP = null;
      function relogin() {
        if (reloginP) return reloginP;
        if (!REMPW) return Promise.resolve(false);
        reloginP = login(REMPW, false).then(
          function (ok) {
            reloginP = null;
            if (!ok) {
              REMPW = "";
              lsDel("uyanik.pw");
            }
            return ok;
          },
          function () {
            reloginP = null;
            return false;
          },
        );
        return reloginP;
      }

      var loginErr = null;
      function login(pw, remember) {
        return raw("/api/login", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ pass: pw }),
        })
          .then(function (r) {
            return r.json().then(
              function (d) {
                return { s: r.status, d: d || {} };
              },
              function () {
                return { s: r.status, d: {} };
              },
            );
          })
          .then(
            function (x) {
              if (x.s === 200 && x.d.ok && x.d.token) {
                loginErr = null;
                loggedOut = false;
                TOKEN = x.d.token;
                lsSet("uyanik.tok", TOKEN);
                if (remember) {
                  REMPW = pw;
                  lsSet("uyanik.pw", pw);
                }
                return true;
              }
              loginErr =
                x.s === 429
                  ? { k: "gate.locked", a: [x.d.wait || 30] }
                  : { k: "gate.bad" };
              return false;
            },
            function () {
              loginErr = { k: "gate.netfail" };
              return false;
            },
          );
      }

      var loggedOut = false;

      function showGate() {
        $("#boot").hidden = true;
        if (!$("#gate").hidden) return;
        TOKEN = "";
        lsDel("uyanik.tok");
        $("#shell").hidden = true;
        $("#gate").hidden = false;
        setTimeout(function () {
          $("#g-pw").focus();
        }, 60);
      }

      function hideGate() {
        $("#boot").hidden = true;
        if (loggedOut) return;
        if ($("#gate").hidden && !$("#shell").hidden) return;
        $("#gate").hidden = true;
        $("#shell").hidden = false;
        $("#g-pw").value = "";
        gmsg(null);

        show("v-dev");
      }

      var gmsgState = null;
      function gmsg(k, a, err) {
        gmsgState = k ? { k: k, a: a, err: err } : null;
        renderGmsg();
      }
      function renderGmsg() {
        var e = $("#g-msg");
        e.textContent = gmsgState ? t(gmsgState.k, gmsgState.a) : "";
        e.className = "note" + (gmsgState && gmsgState.err ? " warn" : "");
      }

      function submitLogin() {
        var pw = $("#g-pw").value;
        if (!pw) {
          gmsg("gate.empty", null, 1);
          return;
        }
        var btn = $("#g-go"),
          rem = $("#g-rem").checked;
        btn.disabled = true;
        gmsg("gate.trying");
        login(pw, rem).then(function (ok) {
          btn.disabled = false;
          if (!ok) {
            gmsg(loginErr.k, loginErr.a, 1);
            return;
          }
          if (!rem) {
            REMPW = "";
            lsDel("uyanik.pw");
          }
          gmsg(null);
          refresh();
        });
      }
      $("#g-go").addEventListener("click", submitLogin);
      $("#g-pw").addEventListener("keydown", function (e) {
        if (e.key === "Enter") submitLogin();
      });

      $("#logout").addEventListener("click", function () {
        
        if (!confirm(t("cfg.logoutAsk"))) return;

        raw("/api/logout", { method: "POST" }).catch(function () {});
        loggedOut = true;
        TOKEN = "";
        REMPW = "";
        lsDel("uyanik.tok");
        lsDel("uyanik.pw");
        lastData = null;
        showGate();
      });

      /* NOTIFICATION */
      var tT = null,
        busy = false;
      var lastClosing = 0,
        lastSsid = "",
        lastMdns = "",
        lastIp = "",
        lastApSsid = "";
      var lastData = null,
        offline = false;

      function rawToast(msg, err) {
        $("#tmsg").textContent = msg;
        var e = $("#toast");
        e.className = "toast show" + (err ? " err" : "");
        clearTimeout(tT);
        tT = setTimeout(function () {
          e.className = "toast" + (err ? " err" : "");
        }, 2600);
      }
      function toast(k, a, err) {
        rawToast(t(k, a), err);
      }

      /* STATUS */
      function paint(d) {
        lastData = d;
        offline = false;
        hideGate();

        var on = d.state === "on",
          el = $("#device");
        el.classList.toggle("on", on);
        el.classList.remove("down");
        $("#sw").setAttribute("aria-checked", on);
        if (!busy) $("#sub").textContent = t(on ? "dev.on" : "dev.off");
        $("#pcname").textContent = d.pcName || t("dev.pc");

        var lv = $("#live"),
          tx = $("#livetxt");
        if (d.portal) {
          lv.className = "live off";
          tx.textContent = t("live.hotspot");
        } else if (d.remoteOk) {
          lv.className = "live";
          tx.textContent = t("live.online");
        } else {
          lv.className = "live off";
          tx.textContent = t("live.local");
        }

        lastClosing = d.closingIn || 0;
        lastSsid = d.wifiSsid || "";
        lastMdns = d.mdns || "";
        lastIp = d.localIp || "";
        lastApSsid = d.apSsid || lastApSsid;
        setMode(d);
        renderBanner();

        if (d.portal) {
          $("#w-cur").textContent = t("wifi.noneT");
          $("#w-cur2").textContent = t("wifi.portalCur");
        } else if (d.wifiSsid) {
          $("#w-cur").textContent = d.wifiSsid;
          $("#w-cur2").textContent = t("wifi.connOn");
        } else {
          $("#w-cur").textContent = t("wifi.noneT");
          $("#w-cur2").textContent = t("wifi.none");
        }

        var sg = $("#w-sig"),
          lit = d.wifiSsid && !d.portal;
        sg.hidden = !lit;
        if (lit) {
          var lv = level(d.rssi || -100);
          for (var i = 0; i < 4; i++)
            sg.children[i].className = i < lv ? "f" : "";
        }

        $("#rssi").textContent = (d.rssi || 0) + " dBm";
        $("#lip").textContent = d.localIp || "-";
        $("#wip").textContent = d.publicIp || "-";
        var s = d.uptime || 0;
        $("#up").textContent = t("conn.uptime", [
          Math.floor(s / 3600),
          Math.floor((s % 3600) / 60),
        ]);

        renderCfgState(d);
      }

      function down() {
        offline = true;
        var el = $("#device");
        el.classList.remove("on");
        el.classList.add("down");
        $("#sw").setAttribute("aria-checked", "false");
        if (!busy) $("#sub").textContent = t("dev.unreachable");
        $("#live").className = "live off";
        $("#livetxt").textContent = t("live.offline");
        renderBanner();
      }

      function renderBanner() {
        var el = $("#hs"),
          d = lastData || {};

        if (factoryDone) {
          return bnr("", "hs.fxT", null, "hs.fxP", [
            esc(lastApSsid || "UYANIK-…"),
          ]);
        }
        if (restarting) return bnr(" info", "hs.rsT", null, "hs.rsP", null);
        if (forgot && (offline || d.apUp)) {
          return bnr("", "hs.forgotT", null, "hs.forgotP", [
            esc(lastApSsid || "UYANIK-…"),
          ]);
        }
        if (offline) {

          if (lastClosing > 0)
            return bnr(" info", "hs.closedT", null, "hs.closedP", [
              esc(lastSsid),
              esc(lastMdns),
              esc(lastIp),
            ]);
          return; /* baska durumda afis oldugu gibi kalir */
        }
        if (d.closingIn > 0) {

          return bnr(" info", "hs.closingT", [d.closingIn], "hs.closingP", [
            esc(d.wifiSsid),
            esc(d.mdns),
            esc(d.localIp),
          ]);
        }
        if (d.portal) {
          return bnr("", "hs.portalT", null, "hs.portalP", [esc(d.apSsid)]);
        }
        el.hidden = true;
      }

      function bnr(cls, tk, ta, pk, pa) {
        var el = $("#hs");
        el.hidden = false;
        el.className = "banner" + cls;
        $("#hs-t").textContent = t(tk, ta);
        $("#hs-p").innerHTML = t(pk, pa);
      }

      function refresh() {
        return api("/api/status")
          .then(function (r) {
            if (!r.ok) throw 0;
            return r.json();
          })
          .then(paint, function () {
            $("#boot").hidden = true;

            if (!$("#gate").hidden) return;
            $("#shell").hidden = false;
            down();
          });
      }

      /* COMMANDS */
      function send(el, cmd, ms, noteKey) {
        busy = true;
        el.disabled = true;
        $("#sub").textContent = t(noteKey);
        api("/api/cmd?c=" + cmd, { method: "POST" })
          .then(
            function (r) {
              if (r.status === 409) toast("cmd.busy", null, 1);
              else if (!r.ok) throw 0;
              else toast("cmd.ok");
            },
            function () {
              throw 0;
            },
          )
          .catch(function () {
            toast("cmd.fail", null, 1);
          })
          .then(function () {
            setTimeout(function () {
              busy = false;
              el.disabled = false;
              refresh();
            }, ms + 900);
          });
      }

      $("#sw").addEventListener("click", function () {
        var el = $("#device");
        el.classList.toggle("on");
        this.setAttribute("aria-checked", el.classList.contains("on"));
        send(this, "power_short", 300, "dev.pressing");
      });

      document.querySelectorAll(".cmd").forEach(function (el) {
        var ms = +el.dataset.ms,
          cmd = el.dataset.cmd;

        if (!el.dataset.hold) {
          el.addEventListener("click", function () {
            send(el, cmd, ms, "dev.resetting");
          });
          return;
        }

        var timer = null,
          tick = null,
          lbl = $("#holdlbl"),
          bar = $("#holdbar");

        function start(e) {
          e.preventDefault();
          if (el.disabled) return;
          var left = 6;
          lbl.textContent = t("cmd.holding", [left]);
          bar.style.transition = "none";
          bar.style.width = "0";
          void bar.offsetWidth; // stil akisini zorla: rAF arka planda durur
          bar.style.transition = "width " + ms + "ms linear";
          bar.style.width = "100%";
          tick = setInterval(function () {
            if (--left > 0) lbl.textContent = t("cmd.holding", [left]);
          }, 1000);
          timer = setTimeout(function () {
            timer = null;
            reset();
            send(el, cmd, ms, "dev.holding");
          }, ms);
        }
        function reset() {
          clearInterval(tick);
          lbl.textContent = t("cmd.forceIdle");
          bar.style.transition = "width .25s";
          bar.style.width = "0";
        }
        function stop() {
          if (!timer) return;
          clearTimeout(timer);
          timer = null;
          reset();
          toast("cmd.released",null,1);
        }
        el.addEventListener("pointerdown", start);
        ["pointerup", "pointerleave", "pointercancel"].forEach(function (n) {
          el.addEventListener(n, stop);
        });
        el.addEventListener("contextmenu", function (e) {
          e.preventDefault();
        });
      });

      /* NAV */
      var view = "v-dev";
      function show(id) {
        view = id;
        document.querySelectorAll(".view").forEach(function (v) {
          v.classList.toggle("on", v.id === id);
        });
        document.querySelectorAll("#rail button").forEach(function (b) {
          b.classList.toggle("on", b.dataset.view === id);
        });
        $("#vtitle").textContent = t("nav." + id.slice(2));
        window.scrollTo(0, 0);
        if (id === "v-cfg") loadCfg();
        if (id === "v-wifi" && !$("#w-list").children.length) scan();
      }
      document.querySelectorAll("#rail button").forEach(function (b) {
        b.addEventListener("click", function () {
          show(b.dataset.view);
        });
      });

      var wasPortal = null;
      function setMode(d) {

        var ap = !!d.apUp;
        $("#w-forget").hidden = ap || !d.wifiSsid;
        if (ap === wasPortal) return;
        wasPortal = ap;
        $("#rail").hidden = ap;

        (ap ? $("#wififx") : $("#cfgfx")).appendChild($("#p-fx"));

        if (ap) {
          show("v-wifi");
          if (!forgot) wmsg(null);
        } else {
          forgot = false;
          show("v-dev");
        }
      }

      /* WIFI*/
      var pick = null,
        scanT = null,
        scanBusy = false,
        netCache = null,
        encrypted = true;

      function level(r) {
        return r >= -55 ? 4 : r >= -67 ? 3 : r >= -78 ? 2 : 1;
      }

      function drawNets(list) {
        netCache = list;
        var box = $("#w-list"),
          form = $("#w-form");

        $("#w-formhome").appendChild(form);
        box.innerHTML = "";

        var cur = lastData && !lastData.portal ? lastData.wifiSsid : "";
        var seen = {},
          rows = [];
        list
          .slice()
          .sort(function (a, b) {
            return b.r - a.r;
          })
          .forEach(function (n) {
            if (!n.s || n.s === cur) return;
            if (seen[n.s]) return;
            seen[n.s] = 1;
            rows.push(n);
          });

        if (!rows.length) {
          form.hidden = true;
          var p = document.createElement("p");
          p.className = "note";

          p.textContent = t(list.length ? "wifi.onlyCurrent" : "wifi.empty");
          box.appendChild(p);
          return;
        }

        var selBtn = null;
        rows.forEach(function (n) {
          var lv = level(n.r),
            b = document.createElement("button");
          b.className = "net" + (pick === n.s ? " sel" : "");
          var sig = "";
          for (var i = 1; i <= 4; i++)
            sig += '<i class="' + (i <= lv ? "f" : "") + '"></i>';
          b.innerHTML =
            "<b></b>" +
            (n.e
              ? '<span class="lock"><svg><use href="#i-lock"/></svg></span>'
              : "") +
            '<span class="sig">' +
            sig +
            "</span>";
          b.querySelector("b").textContent = n.s;
          b.addEventListener("click", function () {

            if (pick !== n.s) $("#w-pw").value = "";
            pick = n.s;
            encrypted = !!n.e;
            drawNets(list);
            if (n.e) $("#w-pw").focus();
          });
          box.appendChild(b);
          if (pick === n.s) selBtn = b;
        });

        if (selBtn) {
          box.insertBefore(form, selBtn.nextSibling);
          form.hidden = false;
          $("#w-pw").parentNode.hidden = !encrypted;
        } else {
          form.hidden = true;
        }
      }

      function scan() {
        var b = $("#w-scan");
        scanBusy = true;
        b.disabled = true;
        b.classList.add("busy");
        b.querySelector("span").textContent = t("wifi.scanning");
        api("/api/wifi/scan", { method: "POST" }).then(function () {
          clearTimeout(scanT);
          scanT = setTimeout(pollScan, 1500);
        }, scanDone);
      }
      function pollScan() {
        api("/api/wifi/scan")
          .then(function (r) {
            return r.json();
          })
          .then(function (d) {
            if (d.scanning) {
              scanT = setTimeout(pollScan, 1200);
              return;
            }
            drawNets(d.nets || []);
            scanDone();
          }, scanDone);
      }
      function scanDone() {
        var b = $("#w-scan");
        scanBusy = false;
        b.disabled = false;
        b.classList.remove("busy");
        b.querySelector("span").textContent = t("wifi.scan");
      }
      $("#w-scan").addEventListener("click", scan);

      var wmsgState = null;
      function wmsg(k, a, err) {
        wmsgState = k ? { k: k, a: a, err: err } : null;
        renderWmsg();
      }
      function wmsgResult(d, err) {
        wmsgState = {
          res: d.wifiResult,
          ssid: d.wifiSsid,
          portal: d.portal,
          err: err,
        };
        renderWmsg();
      }
      function wmsgText() {
        if (!wmsgState) return "";
        if (wmsgState.k) return t(wmsgState.k, wmsgState.a);
        var m = RESULT[wmsgState.res] || ["res.other", 1];
        var s = t(m[0], m[0] === "res.ok" ? [wmsgState.ssid || ""] : null);
        if (wmsgState.res !== "ok" && wmsgState.portal)
          s += " " + t("res.portal");
        return s;
      }
      function renderWmsg() {
        var e = $("#w-msg");
        e.textContent = wmsgText();
        e.className = "note" + (wmsgState && wmsgState.err ? " warn" : "");
      }

      $("#w-go").addEventListener("click", function () {
        if (!pick) return;
        var btn = this;
        btn.disabled = true;
        wmsg("wifi.trying");
        api("/api/wifi/connect", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ ssid: pick, pass: $("#w-pw").value }),
        }).then(
          function () {

            setTimeout(function () {
              watchConnect(0);
            }, 3000);
          },
          function () {
            wmsg("wifi.sendFail", null, 1);
            btn.disabled = false;
          },
        );
      });

      var RESULT = {
        ok: ["res.ok", 0],
        sifre: ["res.sifre", 1],
        bulunamadi: ["res.bulunamadi", 1],
        "zaman asimi": ["res.zamanasimi", 1],
        "ad-bos": ["res.adbos", 1],
        "ad-uzun": ["res.aduzun", 1],
        "sifre-uzun": ["res.sifreuzun", 1],
      };

      function watchConnect(n) {
        api("/api/status")
          .then(function (r) {
            return r.json();
          })
          .then(
            function (d) {
              if (d.wifiResult === "deneniyor" && n < 15) {
                wmsg("wifi.tryingN", [n * 2 + 3]);
                setTimeout(function () {
                  watchConnect(n + 1);
                }, 2000);
                return;
              }
              $("#w-go").disabled = false;

              var m = RESULT[d.wifiResult] || ["res.other", 1];
              wmsgResult(d, m[1]);
              rawToast(wmsgText(), m[1]);

              paint(d);
            },
            function () {

              wmsg("wifi.moved", null, 1);
              $("#w-go").disabled = false;
            },
          );
      }

      var forgot = false;
      $("#w-forget").addEventListener("click", function () {
        if (!confirm(t("wifi.forgetAsk"))) return;

        forgot = true;
        renderBanner();
        api("/api/wifi/forget", { method: "POST" }).catch(function () {});
      });

      /* FACTORY RESET */
      var factoryDone = false;
      (function () {
        var el = $("#fx"),
          bar = $("#fxbar"),
          timer = null;
        function stop() {
          if (!timer) return;
          clearTimeout(timer);
          timer = null;
          bar.style.transition = "width .2s";
          bar.style.width = "0";
        }
        el.addEventListener("pointerdown", function (e) {
          e.preventDefault();
          bar.style.transition = "none";
          bar.style.width = "0";
          void bar.offsetWidth;
          bar.style.transition = "width 3000ms linear";
          bar.style.width = "100%";
          timer = setTimeout(function () {
            timer = null;
            bar.style.width = "0";
            factoryDone = true;
            renderBanner();
            window.scrollTo(0, 0);
            api("/api/factory", { method: "POST" }).catch(function () {});
          }, 3000);
        });
        ["pointerup", "pointerleave", "pointercancel"].forEach(function (n) {
          el.addEventListener(n, stop);
        });
        el.addEventListener("contextmenu", function (e) {
          e.preventDefault();
        });
      })();

      /* SETTINGS */
      var cfgErr = "";
      function renderCfgErr() {
        var e = $("#c-err");
        e.textContent = terr(cfgErr);
        e.className = "note" + (cfgErr ? " warn" : "");
      }

      function renderCfgState(d) {
        var e = $("#c-state"),
          sp = e.querySelector("span");
        var bad =
          d.configError || (d.remote && d.remote !== "ok" ? d.remote : "");
        if (d.remoteOk) {
          e.className = "state ok";
          sp.textContent = t("cfg.remoteOk");
        } else if (bad) {
          e.className = "state bad";
          sp.textContent = t("cfg.remoteOff") + " " + terr(bad);
        } else if (!d.clock) {

          e.className = "state";
          sp.textContent = t("cfg.remoteClock");
        } else {
          e.className = "state";
          sp.textContent = t("cfg.remoteWait");
        }
      }

      function loadCfg() {
        api("/api/settings")
          .then(function (r) {
            return r.json();
          })
          .then(
            function (d) {
              $("#c-name").value = d.deviceName || "";
              $("#c-key").value = d.apiKey || "";
              $("#c-url").value = d.dbUrl || "";
              $("#c-mail").value = d.email || "";
              $("#c-pcname").value = d.pcName || "";
              $("#c-apsfx").value = d.apSuffix || "";
              $("#c-appw").value = "";
              $("#c-pw").value = "";
              cfgErr = d.error || "";
              renderCfgErr();
            },
            function () {
              toast("cfg.readFail", null, 1);
            },
          );
      }

      document.querySelectorAll("[data-save]").forEach(function (btn) {
        btn.addEventListener("click", function () {
          var all = document.querySelectorAll("[data-save]");
          all.forEach(function (b) {
            b.disabled = true;
          });
          api("/api/settings", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
              deviceName: $("#c-name").value.trim(),
              apiKey: $("#c-key").value.trim(),
              dbUrl: $("#c-url").value.trim(),
              email: $("#c-mail").value.trim(),
              pcName: $("#c-pcname").value.trim(),
              apSuffix: $("#c-apsfx").value.trim(),
              apPw: $("#c-appw").value,
              password: $("#c-pw").value,
            }),
          })
            .then(function (r) {
              return r.json();
            })
            .then(
              function (d) {
                if (d.ok) {
                  toast("cfg.saved");
                  loadCfg();
                  refresh();
                } else {
                  cfgErr = d.error || "";
                  renderCfgErr();
                  rawToast(terr(d.error) || t("cfg.saveFail"), 1);
                }
              },
              function () {
                toast("cfg.saveFail", null, 1);
              },
            )
            .then(function () {
              all.forEach(function (b) {
                b.disabled = false;
              });
            });
        });
      });

      /* RESTART */
      var restarting = false;
      $("#rs").addEventListener("click", function () {
        if (!confirm(t("cfg.rsAsk"))) return;
        restarting = true;
        renderBanner();
        window.scrollTo(0, 0);
        api("/api/restart", { method: "POST" }).catch(function () {});

        setTimeout(function () {
          restarting = false;
          refresh();
        }, 12000);
      });

      /* PANEL PASSWORD */
      var pmsgState = null;
      function pmsg(k, a, err) {
        pmsgState = k ? { k: k, a: a, err: err } : null;
        renderPmsg();
      }
      function pmsgRaw(txt, err) {
        pmsgState = txt ? { raw: txt, err: err } : null;
        renderPmsg();
      }
      function renderPmsg() {
        var e = $("#p-msg");
        if (!e) return;
        e.textContent = pmsgState
          ? pmsgState.raw || t(pmsgState.k, pmsgState.a)
          : "";
        e.className = "note" + (pmsgState && pmsgState.err ? " warn" : "");
      }

      $("#p-go").addEventListener("click", function () {
        var btn = this,
          next = $("#p-new").value;
        btn.disabled = true;
        pmsg(null);
        api("/api/panelpw", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ current: $("#p-cur").value, next: next }),
        })
          .then(function (r) {
            return r.json();
          })
          .then(
            function (d) {
              btn.disabled = false;
              if (!d.ok) {
                pmsgRaw(terr(d.error) || t("cfg.saveFail"), 1);
                return;
              }

              if (d.token) {
                TOKEN = d.token;
                lsSet("uyanik.tok", TOKEN);
              }
              if (REMPW) {
                REMPW = next;
                lsSet("uyanik.pw", next);
              }

              $("#p-cur").value = "";
              $("#p-new").value = "";
              pmsg("cfg.ppwOk");
              toast("cfg.ppwOk");
            },
            function () {
              btn.disabled = false;
              pmsg("cfg.saveFail", null, 1);
            },
          );
      });

      /* SHOW PASSWORD */
      document.querySelectorAll("[data-eye]").forEach(function (b) {
        b.addEventListener("click", function () {
          var inp = document.getElementById(b.dataset.eye);
          var hid = inp.type === "password";
          inp.type = hid ? "text" : "password";
          b.classList.toggle("on", hid);
          b.setAttribute("aria-label", t(hid ? "a.hide" : "a.show"));
          b.querySelector("use").setAttribute(
            "href",
            hid ? "#i-eye-off" : "#i-eye",
          );
          inp.focus();
        });
      });

      /* OPENING*/
      applyLang();
      refresh();
      setInterval(function () {
        refresh();
      }, 4000);

      setInterval(function () {
        if (lastClosing > 0 && !offline) refresh();
      }, 1000);
    </script>
  </body>
</html>)HTMLPAGE";
