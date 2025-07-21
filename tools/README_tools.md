# 🔧 CH549 燒錄工具使用說明

本目錄提供一系列適用於 CH549 的燒錄工具與相關資源，支援 SDCC 編譯器、WCH 官方命令行工具、Linux USB 驅動等。

## 📁 目錄結構

```plaintext
tools/
├── header_for_sdcc/                  // SDCC 頭文件
│   ├── CH549_sdcc.h
│   └── compiler.h
├── README_tools.md                   // 本說明文件
├── wchisp-linux-x64/                 // wchisp 燒錄工具
│   ├── README.md
│   └── wchisp                        // 可執行檔
└── WCH-linux-x64/                    // WCH 官方命令行工具與驅動
    ├── ch37x.ko                      // USB 驅動模組（Linux）
    ├── CH549.INI                     // 燒錄設定檔（由 Windows 工具生成）
    ├── README.md
    ├── WCHISPTool_CMD                // 可執行檔
    ├── WCHISPTool_CMD Command Line Programming Tool Instruction.pdf
    ├── WCHISPTool_CMD 命令行烧录工具使用说明.pdf
    └── WCHISPTool_CMD.ZIP            // 原始碼與驅動指導文件
```

## 🧩 安裝與設定說明

### 1. SDCC 頭文件安裝

頭文件複製至 SDCC mcs51 的標頭檔案路徑：

```bash
cp header_for_sdcc/*.h /usr/local/bin/share/sdcc/include/mcs51/
```

---

### 2. 燒錄工具：`wchisp`

- 在工程根目錄下執行燒錄：
  ```bash
  make wchisp_flash
  ```
- 避免衝突，使用 `wchisp` 時，請 **卸載 ch37x USB 驅動**：
  ```bash
  make disable_ch37x
  ```

---

### 3. 官方工具：`WCHISPTool_CMD`

- 在工程根目錄下執行燒錄：
  ```bash
  make WCH_flash
  ```
- 使用前，請 **載入 ch37x USB 驅動**：
  ```bash
  make enable_ch37x
  ```
- ch37x USB 驅動 對應不同版本的 Linux 核心，要自行編繹。
- 該子目錄包含中英文說明文件，與及 ZIP 壓縮的原始碼與驅動模組。
- `CH549.INI` 由 Windows 版本的 ISP 工具產生，可用於 WCHISPTool_CMD。

---

## 📘 補充資訊

- `WCHISPTool_CMD.ZIP` 內含完整原始碼與 Linux 驅動編譯指導，請依所使用的 Linux 版本進行相容性處理。
- 所有工具與執行檔均已在 x64 Linux 環境下測試。
- 官方原始碼 [`README.md`](./WCH-linux-x64/README.md) 中包含自訂功能與使用建議，可依需求客製化。
