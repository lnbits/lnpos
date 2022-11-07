const stringifyNice = (data) => JSON.stringify(data, null, 2)
const templates = Object.freeze({
    beskomat: {
        fileName: "beskomat.txt",
        value: `apiKey.id=f4bae82c8e4c4a2d
apiKey.key=3cea6a582c3d6dc81b72879b1ac090cc5ad42b4265537f448f60fccfc50705c0
apiKey.encoding=hex
fiatCurrency=EUR
callbackUrl=https://legend.lnbits.com/bleskomat/u
shorten=true`,
    },
    elements: {
        fileName: "elements.json",
        value: stringifyNice([
            {
                "name": "password",
                "type": "ACInput",
                "value": "ToTheMoon1",
                "label": "Password for PoS AP WiFi",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "masterkey",
                "type": "ACInput",
                "value": "",
                "label": "Master Public Key",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "server",
                "type": "ACInput",
                "value": "",
                "label": "LNbits Server",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "invoice",
                "type": "ACInput",
                "value": "",
                "label": "Wallet Invoice Key",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "lncurrency",
                "type": "ACInput",
                "value": "",
                "label": "PoS Currency ie EUR",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "lnurlpos",
                "type": "ACInput",
                "value": "",
                "label": "LNURLPoS String",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "lnurlatm",
                "type": "ACInput",
                "value": "",
                "label": "LNURLATM String",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "lnurlatmms",
                "type": "ACInput",
                "value": "mempool.space",
                "label": "mempool.space server",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            },
            {
                "name": "lnurlatmpin",
                "type": "ACInput",
                "value": "878787",
                "label": "LNURLATM pin String",
                "pattern": "",
                "placeholder": "",
                "style": "",
                "apply": "text"
            }
        ])
    }
})

