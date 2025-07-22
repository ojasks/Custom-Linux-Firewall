# Kernel Recon Shield

<img width="834" height="553" alt="Custom_linux_Firewall" src="https://github.com/user-attachments/assets/865ecb49-b9f3-457b-8b2a-d1597ccb894f" />


A custom Linux-based firewall tool built for deep packet inspection, rule-based traffic control, and protocol anomaly detection using Netfilter and low-level kernel hooks.

## 🔍 Overview

Kernel Recon Shield is designed to analyze and filter packets at the kernel level using Netfilter hooks. It aims to give developers and security enthusiasts more visibility and control over network traffic by handling packet inspection directly in the Linux kernel.

This is a learning-focused repo where core concepts like handling TCP flags (ACK, XMAS, NULL), rule enforcement, and basic logging are implemented in a clean, modular way.

## 🚀 Features Implemented

- Netfilter hook integration at `NF_INET_PRE_ROUTING`
- Packet inspection logic in kernel space
- Detection and logging of suspicious TCP flags (ACK, NULL, XMAS scans)
- Rule-based filtering framework
- Logging decisions and suspicious traffic to `dmesg`

## 🔧 Getting Started

> **Note:** Only use this on isolated environments or virtual machines. Kernel modules can crash your system if used incorrectly.

### Clone and Compile
```bash
git clone https://github.com/ojasks/Kernel-Recon-Shield.git
cd Kernel-Recon-Shield
make
