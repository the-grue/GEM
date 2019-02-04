del hc.err
for %%f in (*.c) do hc %%f >>hc.err
for %%f in (*.asm) do masm %%f; >>hc.err
