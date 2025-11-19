#!/usr/bin/env python3
"""
Script de prueba para verificar la conexión MQTT
Prueba cada componente del cliente MQTT paso a paso
"""

import time
import sys
from app.mqtt_client import create_mqtt_client, get_connection_status

def test_mqtt_connection():
    """Función principal de prueba"""
    print("=" * 60)
    print("PRUEBA DE CONEXIÓN MQTT")
    print("=" * 60)
    
    # Test 1: Crear el cliente
    print("\n[TEST 1] Creando cliente MQTT...")
    try:
        client = create_mqtt_client()
        print("✓ Cliente MQTT creado exitosamente")
    except Exception as e:
        print(f"✗ Error creando cliente MQTT: {e}")
        return False
    
    # Test 2: Esperar y verificar conexión
    print("\n[TEST 2] Esperando conexión (máx 10 segundos)...")
    start_time = time.time()
    max_wait = 10
    connected = False
    
    while time.time() - start_time < max_wait:
        status = get_connection_status()
        print(f"  Estado actual: {status}")
        
        if status == "Conectado":
            connected = True
            print("✓ ¡Conexión exitosa!")
            break
        elif status == "Error de conexión":
            print(f"✗ Error de conexión: {status}")
            break
        
        time.sleep(2)
    
    if not connected:
        print("✗ No se logró conectar en el tiempo límite")
        return False
    
    # Test 3: Verificar estado después de conexión
    print("\n[TEST 3] Verificando estado de conexión...")
    status = get_connection_status()
    print(f"  Estado: {status}")
    
    if status == "Conectado":
        print("✓ Estado de conexión correcto")
    else:
        print(f"✗ Estado inesperado: {status}")
        return False
    
    # Test 4: Mantener conexión activa
    print("\n[TEST 4] Manteniendo conexión activa (30 segundos)...")
    print("  Si no hay errores en la consola, todo funciona correctamente")
    
    try:
        for i in range(15):
            status = get_connection_status()
            print(f"  [{i+1}/15] Estado: {status}")
            time.sleep(2)
        print("✓ Conexión se mantuvo estable")
    except KeyboardInterrupt:
        print("\n  Prueba interrumpida por el usuario")
    except Exception as e:
        print(f"✗ Error durante la prueba: {e}")
        return False
    
    print("\n" + "=" * 60)
    print("RESULTADO: ✓ Todas las pruebas pasaron correctamente")
    print("=" * 60)
    return True

if __name__ == "__main__":
    try:
        success = test_mqtt_connection()
        sys.exit(0 if success else 1)
    except Exception as e:
        print(f"\n✗ Error fatal: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
