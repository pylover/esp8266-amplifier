<script>
import { onMount } from 'svelte';
import Button from './Button.svelte';
import Section from './Section.svelte';

let disabled = true;
let p = {};

onMount(async () => {
  const res = await fetch(`/params.json`);
  if (res.ok) {
    p = await res.json();
    //title = document.title = `${p.zone}.${p.name}`;
    disabled = false;
  }
  else {
    throw new Error(res.text());
  }
});

async function submit(event) {
  var formraw = {
    'zone':     p.zone,
    'name':     p.name,
    'ap_psk':   p.apPsk,
    'ssid':     p.ssid,
    'psk':      p.psk,
    'psu':      p.psu
  };

  var form = [];
  for (var property in formraw) {
    var encodedKey = encodeURIComponent(property);
    var encodedValue = encodeURIComponent(formraw[property]);
    form.push(encodedKey + "=" + encodedValue);
  }
  form = form.join("&");

  const res = await fetch('/params', {
    method: 'POST',
    headers: {
    'Content-Type': 'application/x-www-form-urlencoded;charset=UTF-8'
    },
    body: form
  });

}
</script>
<style type="text/sass" scoped>
@import 'styles/variables.sass'

form > .row > div:first-child
    margin-top: $gutter * 2
    margin-bottom: $gutter
</style>

<p style="display: {disabled? 'block': 'none'}" >
  Cannot load params.
</p>
<div id="settings" class="row" style="display: {disabled? 'none': 'block'}">
  <form>
  <Section title="Connection" icon="connection" />
  <div class="row all10">
    <div class="all10">Zone:</div>
    <div class="all10">
      <input name="zone" bind:value="{p.zone}" {disabled}/>
    </div>
  </div>
  <div class="row all10">
    <div class="all10">Name:</div>
    <div class="all10">
      <input name="name" bind:value="{p.name}" {disabled}/>    
    </div>
  </div>
  <div class="row all10">
    <div class="all10">Access Point PSK</div>
    <div class="all10">
      <input name="ap_psk" bind:value="{p.apPsk}" {disabled}/>
    </div>
  </div>
  <div class="row all10">
    <div class="all10">SSID</div>
    <div class="all10">
      <input name="ssid" bind:value="{p.ssid}" {disabled}/>    
    </div>
  </div>
  <div class="row all10">
    <div class="all10">PSK</div>
    <div class="all10">
      <input name="psk" bind:value="{p.psk}" {disabled}/>    
    </div>
  </div>
  <Section title="Power Supply" icon="switch" />
  <div class="row all10">
    <div class="all10">PSU Name</div>
    <div class="all10">
      <input name="psu" bind:value="{p.psu}" {disabled}/>    
    </div>
  </div>
  <div class="row all10">
    <div class="all10">
      <Button title="Save & Reboot" icon="floppy-disk" on:click={submit} />
    </div>
  </div>
  </form> 
</div>
